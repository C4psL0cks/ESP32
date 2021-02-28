#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

int firedigitalPin = 33;
int valfire = 0 ;
int firevalue = 0;
int nodeNumber = 1;

const char* ssid = "KK-1001";
const char* password = "58914472";

// MESH Details
#define   MESH_PREFIX     "RNTMESH" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

//String to send to other nodes with sensor readings
String readings;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
String SendSyncDatatoAllNode(); // Prototype for sending sensor readings

//Create tasks: to send messages and get readings;
Task taskSendMessage(TASK_SECOND * 5 , TASK_FOREVER, &sendMessage);

String SendSyncDatatoAllNode() { // send

  JSONVar jsonReadings;
  jsonReadings["node"] = nodeNumber;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void sendMessage () {
  String msg = SendSyncDatatoAllNode();
  mesh.sendBroadcast(msg);
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {

  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());

  int node = myObject["node"];
  Serial.print("Node: ");
  Serial.println(node);
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
  Serial.begin(115200);
  //  Serial.println("Booting");
  //  WiFi.mode(WIFI_STA);
  //  WiFi.begin(ssid, password);
  //  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //    Serial.println("Connection Failed! Rebooting...");
  //    delay(5000);
  //    ESP.restart();
  //  }
  //
  //  ArduinoOTA
  //  .onStart([]() {
  //    String type;
  //    if (ArduinoOTA.getCommand() == U_FLASH)
  //      type = "sketch";
  //    else // U_SPIFFS
  //      type = "filesystem";
  //
  //    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  //    Serial.println("Start updating " + type);
  //  })
  //  .onEnd([]() {
  //    Serial.println("\nEnd");
  //  })
  //  .onProgress([](unsigned int progress, unsigned int total) {
  //    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //  })
  //  .onError([](ota_error_t error) {
  //    Serial.printf("Error[%u]: ", error);
  //    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //  });
  //
  //  ArduinoOTA.begin();
  //
  //  Serial.println("Ready");
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());

  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  //  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {

  if (analogRead(firedigitalPin) < 4095)
  {
    firevalue = 1;
    Serial.println("FLAME, FLAME, FLAME");
  }
  else
  {
    firevalue = 0;
    Serial.println("no flame");
  }

  ArduinoOTA.handle();
  mesh.update();
  delay(1000);
}
