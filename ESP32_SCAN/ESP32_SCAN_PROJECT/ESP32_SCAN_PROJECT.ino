#include <WiFi.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "time.h"
#include <HTTPClient.h>
#include <TridentTD_LineNotify.h>
#include <Adafruit_Fingerprint.h>

#define LED_STATE   25
#define WIFI_SSID   "dlink-ABD0"
#define WIFI_PASS   "yyafr68490"
#define LINE_TOKEN  "VOwca6uoY6T5vR6A43J7zwg2PtOWG2z4wGvgTLFsN3u"

HTTPClient http;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

struct tm timeinfo;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;
char daysw[10];
char days[10];
char Month[10];
char years[10];
char h[3];
char m[3];
char s[3];
String dataString = "";
String host = "http://192.168.0.105/esp32scan/apis.php";
boolean check = false;

void setup() {

  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  pinMode(LED_STATE, OUTPUT);
  digitalWrite(LED_STATE, LOW);
  Serial.println();
  Serial.println("-------------------------------------");
  Serial.println("Running!");
  Serial.println("-------------------------------------");

  while (!Serial);

  SD.begin(5);
  if (!SD.begin(5)) {
    Serial.println("Card Mount Failed");
    return;
  }

  //  writeFile(SD, "/datalog_1.csv", "ID, NAME, DATETIME \r\n");

  File file = SD.open("/datalog_2.csv");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/datalog_2.csv", "ID, NAME, DATETIME \r\n");
  }
  else {
    Serial.println("File already exists");
  }
  file.close();

  listSD();

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  finger.getTemplateCount();
  Serial.println("Sensor contains: " + String(finger.templateCount) + " templates");
  Serial.println("Waiting for valid finger...");

  // Connect to Wifi.
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("WiFi Connecting to %s\n", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  Serial.println();

  LINE.setToken(LINE_TOKEN);

  LINE.notify("เริ่มต้นการทำงาน....");
  delay(100);

  digitalWrite(LED_STATE, HIGH);

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //  LINE.notify("เริ่มต้นการทำงาน....");
}

void loop() {

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int ID = getFingerprintIDez();

  //  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  strftime(daysw, 10, "%A", &timeinfo);
  strftime(days, 10, "%d", &timeinfo);
  strftime(Month, 10, "%B", &timeinfo);
  strftime(years, 10, "%Y", &timeinfo);
  strftime(h, 3, "%H", &timeinfo);
  strftime(m, 3, "%M", &timeinfo);
  strftime(s, 3, "%S", &timeinfo);


  if (ID > 0) {
    http.begin(host);
    http.addHeader("Content-Type", "application/json");
    String httpRequestData = "{\"id\":\"" + String(ID) + "\",\"time\":\"" + String(daysw) + " " + String(days) + " " + String(Month) + " " + String(years) + "\"}";
    int httpResponseCode = http.POST(httpRequestData);
    Serial.println("---------------------------------------");
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("---------------------------------------");
    if (httpResponseCode == 200) {
      Serial.println("---------------------------------------");
      String payload = http.getString();
      Serial.println(payload);
      Serial.println("---------------------------------------");

      dataString += String(finger.fingerID);
      dataString += ",";
      dataString += String(payload);
      dataString += ",";
      dataString += String(h) + ":" + String(m) + ":" + String(s) + " " + String(daysw) + " " + String(days) + " " + String(Month) + " " + String(years);
      dataString += "\r\n";

      Serial.println("SAVE:" + String(dataString));

      if (check == false && ID != -1) {
        appendFile(SD, "/datalog_2.csv", dataString.c_str());
        digitalWrite(LED_STATE, HIGH);
        LINE.notify("ID:" + String(ID) + "NAME:" + String(payload) + "TIME:" + String(h) + ":" + String(m) + ":" + String(s) + " " + String(daysw) + " " + String(days) + " " + String(Month) + " " + String(years));
        check = true;
      }
    }
    http.end();
  }
  check = false;
  dataString = "";
  //  readFile(SD, "/datalog_2.csv");
  digitalWrite(LED_STATE, LOW);
  delay(1000);
}
int getFingerprintIDez() {

  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.println("---------------------------------------");
  Serial.println("Found ID :" + String(finger.fingerID));
  Serial.println("with confidence of :" + String(finger.confidence));
  Serial.println("---------------------------------------");
  return finger.fingerID;
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void listSD(void) {
  Serial.println(F("\r\nListing SD files:"));
  static const char line[] PROGMEM =  "=================================================";

  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));

  fs::File root = SD.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.println(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }
  Serial.println(FPSTR(line));
  Serial.println();
  delay(1000);
}
