#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


int measurePin = 34;
int ledPower = 33;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustdensity = 0;


const char* ssid = "KK-1001";
const char* password = "58914472";

// MESH Details
#define   MESH_PREFIX     "RNTMESH" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

//Number for this node
int nodeNumber = 2;
float humidity;

//String to send to other nodes with sensor readings
String readings;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings

//Create tasks: to send messages and get readings;
Task taskSendMessage(TASK_SECOND * 5 , TASK_FOREVER, &sendMessage);

String SendSyncDatatoAllNode () { // send
  
  JSONVar jsonReadings;
  humidity = 10;
  JSON.stringify(humidity);
  
  jsonReadings["node"] = nodeNumber;
  jsonReadings["hum"] = humidity;

  readings = JSON.stringify(jsonReadings);
  return readings;
}

void sendMessage () {
  String msg = getReadings();
  mesh.sendBroadcast(msg);
}

void initDHT() {
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["node"];
  double pres = myObject["pres"];
  int smok = myObject["smok"];
  //  int fir = myObject["fir"];
  int ligh = myObject["ligh"];
  //  char wal = myObject["wal"];
  //  char soun = myObject["soun"];
  int firevalue = myObject["firevalue"];

  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" pa");
  Serial.print("Smoke: ");
  Serial.print(smok);
  Serial.println(" ");
  Serial.print("Fire: ");
  Serial.print(firevalue);
  if (firevalue == 1) {
    Serial.println("FLAME, FLAME, FLAME");
  }
  else {
    Serial.println("no flame");
  }
  Serial.println(" ");
  Serial.print("Light Intensity: ");
  Serial.print(ligh);
  Serial.println(" lux");
  Serial.print(" ");
  //  Serial.print(wal);
  //  Serial.print(" ");
  //  Serial.print(soun);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(9600);

  initDHT();

  pinMode(ledPower, OUTPUT);
  //  pinMode(analogPin, INPUT);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustdensity = (0.17 * calcVoltage - 0.1) * 1000;

  Serial.print("Dust Density: ");
  Serial.print(dustdensity); // unit: mg/m3
  Serial.println(" ug/m3");

  // it will run the user scheduler as well
  mesh.update();
  ArduinoOTA.handle();
  delay(2500);
}
