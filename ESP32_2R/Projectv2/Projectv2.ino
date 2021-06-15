#include <time.h>
#include <WiFi.h>
#include <TridentTD_LineNotify.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>

#define FIREBASE_HOST   "db-ionics-tester.firebaseio.com"
#define FIREBASE_AUTH   "QoZE7t6nSIHXKSW7BqQQZvgLyXvhdow4oFKILM8d"
#define WIFI_SSID       "dlink-ABD0"
#define WIFI_PASSWORD   "yyafr68490"
#define LINE_TOKEN      "AdZ7N59UWbEw2ZCEXfMQY7tdIThWowWK4ukHCSZNRhp"

int trigPin = 5;     // Trigger
int echoPin = 18;    // Echo
int diameter = 7;
int heightfix = 10; // 1-11cm 2-11cm 3-10cm
int height = 0;
long duration, cm;
double liter  = 0.0;
boolean check_round = false;
struct tm* p_tm;
char ntp_server1[20] = "pool.ntp.org";
char ntp_server2[20] = "time.nist.gov";
char ntp_server3[20] = "time.uni.net.th";
String times = "";
int Minutes;
String Node = "Node3";


void setup() {

  //Serial Port begin
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println(LINE.getVersion());
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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  LINE.setToken(LINE_TOKEN);

  configTime(7 * 3600, 0, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("\nWaiting for time");

}

void loop() {

  // Time
  time_t time_now = time(nullptr);
  p_tm = localtime(&time_now);
  Minutes = p_tm->tm_min;
  times = ctime(&time_now);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  height = heightfix - cm;

  Serial.println("Cm : " + String(cm));
  Serial.println("Height : " + String(height));
  Serial.println("Times : " + String(times));

  liter = pow((diameter / 2), 2) * 3.14 * height / 1000;
  Serial.println("liter: " + String(liter));

  if (Minutes % 2 == 0) {
    Serial.println("CHECK:" + String(Minutes));
    if (liter > 0.0 && check_round == false) {

      LINE.notify("\nDevice_id : " + String(Node)
                  + "\nLiter : " + String(liter)
                  + "\nHeight : " + String(height)
                  + "\nTimes : " + String(times));

      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& JSONencoder = JSONbuffer.createObject();

      JSONencoder["device_id"] = Node;
      JSONencoder["liter"] = liter;
      JSONencoder["height"] = height;
      JSONencoder["times"] = times;

      Serial.println("------------------------------------");
      Serial.println("JSON Data");
      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      Serial.println(JSONmessageBuffer);
      Serial.println("------------------------------------");

      Firebase.setString("esp32/" + String(Node) + "/device_id", Node);
      Firebase.setFloat("esp32/" + String(Node) + "/liter", liter);
      Firebase.setFloat("esp32/" + String(Node) + "/height", height);
      Firebase.setString("esp32/" + String(Node) + "/times", times);


      check_round = true;
    }
  }
  else {
    Serial.println("RESRT:" + String(Minutes));
    check_round = false;
  }
  delay(1000);
}
