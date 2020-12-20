#include "FS.h"
#include "SD.h"
#include "SPI.h"


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

void setup() {
  Serial.begin(115200);

  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  writeFile(SD, "/datalog_ESP32.txt", "Time, Humidity, Temperature \r\n");
}

void loop() {

  String dataString = "";

  float h = 0;
  float t = 10;

  dataString += String(20);
  dataString += ",";
  dataString += String(h);
  dataString += ",";
  dataString += String(t);
  dataString += "\r\n";

  Serial.println(dataString);
  appendFile(SD, "/datalog_ESP32.txt", dataString.c_str());

  delay(2000);
}
