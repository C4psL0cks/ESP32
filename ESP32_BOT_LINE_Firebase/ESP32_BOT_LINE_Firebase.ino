#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <time.h>

#define Plug 23

// Config Firebase
#define FIREBASE_HOST "plug-htjrgv.firebaseio.com"
#define FIREBASE_AUTH "d63Y0rAWzky91L6Cyh3PJLD4GRWNxMiesIKlL7px"

// Config connect WiFi
#define WIFI_SSID "6021607"
#define WIFI_PASSWORD "17401449"

/// time
struct tm* p_tm;
char ntp_server1[20] = "pool.ntp.org";
char ntp_server2[20] = "time.nist.gov";
char ntp_server3[20] = "time.uni.net.th";
String times = "";
int sensor1;
float sums;
int plug;
String state = "";


void setup() {

  Serial.begin(115200);
  pinMode(Plug, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  configTime(7 * 3600, 0, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {


  // Time
  time_t time_now = time(nullptr);
  p_tm = localtime(&time_now);
  times = ctime(&time_now);
  Serial.println(times);

  // append a new value to /logs
  sensor1 = analogRead(A0);
  sums = (sensor1 * 5.5 / 4095);
  Serial.println("STATE SENSOR:" + String(sums));
  if (sums > 0) {
    state = "NO";
  } else {
    state = "OFF";
  }

  // append a new value to /logs sensor
  Firebase.pushString("data1/state", state);

  // append a new value to /logs now time
  Firebase.pushString("data2/time", NowTime());

  // append a new value to /logs now date
  Firebase.pushString("data2/date", NowDate());

  // get plug state
  plug = Firebase.getInt("data3/PLUG/state/");
  Serial.println("STATE PLUG:" + String(plug));

  // set plug relay active low on
  digitalWrite(Plug, (plug == 0 ? HIGH : LOW));

  if (Firebase.failed()) {
    Serial.println("Firebase Failed");
    Serial.println(Firebase.error());
  }
  delay(500);

}

String NowTime() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  String timeNow = "";
  timeNow += String(p_tm->tm_hour);
  timeNow += ":";
  timeNow += String(p_tm->tm_min);
  timeNow += ":";
  timeNow += String(p_tm->tm_sec);
  timeNow += " ";
  return timeNow;
}

String NowDate() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  String dateNow = "";
  dateNow += String(p_tm->tm_mday);
  dateNow += "-";
  dateNow += String(p_tm->tm_mon + 1);
  dateNow += "-";
  dateNow += String(p_tm->tm_year + 1900);
  return dateNow;
}
