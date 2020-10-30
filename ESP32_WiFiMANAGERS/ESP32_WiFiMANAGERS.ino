//needed for library
#include <FS.h>
#include <SPIFFS.h>

//needed for library
#include <ArduinoOTA.h>

//needed for library
#include <WiFi.h>
#include <WiFiClient.h>

//needed for library
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//for LED status
#include <Ticker.h>
Ticker ticker;

#define buttonPIN  19
#define BUILTIN_LED  2
#define EEPROM_SALT 12663
#define AUTO     0
#define MANUAL   1

float TempLOW = 0.0;
float TempHIGH = 0.0;
float HumiLOW = 0.0;
float HumiHIGH = 0.0;
float Temp = 0.0;
float Humi = 0.0;
unsigned long startPress = 0;
const char *hostname = "WifiFishSun";

typedef struct {
  int   salt = EEPROM_SALT;
  char  blynkToken[33]  = "";
  char  blynkServer[33] = "blynk-cloud.com";
  char  blynkPort[6]    = "8080";
} WMSettings;

WMSettings settings;

WiFiManager wifiManager;

//flag for saving data
bool shouldSaveConfig = false;

void tick() {
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);

  Serial.println("  Enter Config Wifi ");
}

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void resetFactory() {
  //reset settings to defaults

  WMSettings defaults;
  settings = defaults;
  EEPROM.begin(512);
  EEPROM.put(0, settings);
  EEPROM.end();

  delay(2000);
  //reset wifi credentials
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.persistent(true);

  delay(2000);
  ESP.restart();
  delay(500);
}

void setup()
{
  Serial.begin(115200);

  WiFiManager wifiManager;

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(buttonPIN, INPUT_PULLUP);

  ticker.attach(0.6, tick);
  Serial.println("     Scan Wifi      ");


  //reset saved settings
  wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //timeout - this will quit WiFiManager if it's not configured in 3 minutes, causing a restart
  wifiManager.setConfigPortalTimeout(180);

  //custom params
  EEPROM.begin(512);
  EEPROM.get(0, settings);
  EEPROM.get(100, TempLOW);
  EEPROM.get(110, TempHIGH);
  EEPROM.get(120, HumiLOW);
  EEPROM.get(130, HumiHIGH);
  EEPROM.end();

  if (settings.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    WMSettings defaults;
    settings = defaults;
  }

  Serial.println(settings.blynkToken);
  Serial.println(settings.blynkServer);
  Serial.println(settings.blynkPort);

  //WiFiManagerParameter custom_blynk_text("Blynk config. <br/> No token to disable.");
  //wifiManager.addParameter(&custom_blynk_text);

  WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", settings.blynkToken, 33);
  wifiManager.addParameter(&custom_blynk_token);

  WiFiManagerParameter custom_blynk_server("blynk-server", "blynk server", settings.blynkServer, 33);
  wifiManager.addParameter(&custom_blynk_server);

  WiFiManagerParameter custom_blynk_port("blynk-port", "blynk port", settings.blynkPort, 6);
  wifiManager.addParameter(&custom_blynk_port);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  if (!wifiManager.autoConnect("ESP32", "123456")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  Serial.println(custom_blynk_token.getValue());
  Serial.println(custom_blynk_server.getValue());
  Serial.println(custom_blynk_port.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("Saving config");

    strcpy(settings.blynkToken, custom_blynk_token.getValue());
    strcpy(settings.blynkServer, custom_blynk_server.getValue());
    strcpy(settings.blynkPort, custom_blynk_port.getValue());

    Serial.println(settings.blynkToken);
    Serial.println(settings.blynkServer);
    Serial.println(settings.blynkPort);

    EEPROM.begin(512);
    EEPROM.put(0, settings);
    EEPROM.end();
  }

  //  //config blynk
  //  if (strlen(settings.blynkToken) == 0) {
  //    BLYNK_ENABLED = false;
  //  }
  //  if (BLYNK_ENABLED) {
  //    Blynk.config(settings.blynkToken, settings.blynkServer, atoi(settings.blynkPort));
  //  }

  //OTA
  //  ArduinoOTA.onStart([]() {
  //    Serial.println("Start OTA");
  //  });
  //  ArduinoOTA.onEnd([]() {
  //    Serial.println("\nEnd");
  //  });
  //  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //  });
  //  ArduinoOTA.onError([](ota_error_t error) {
  //    Serial.printf("Error[%u]: ", error);
  //    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //  });
  //  ArduinoOTA.setHostname(hostname);
  //  ArduinoOTA.begin();

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.println("   Connect Finish   ");
  Serial.println("    !! Online !!    ");
  Serial.println("      Start..       ");
}

void loop() {

  //  ArduinoOTA.handle();

  Serial.println("Run.....");
  startPress = millis();

  while (digitalRead(buttonPIN) == LOW) {
    unsigned long duration = millis() - startPress;
    if (duration > 4000) {
      Serial.println("    Reset Factory   ");
      Serial.println("       Reboot       ");
      delay(2000);
      resetFactory();
    }
  }

  delay(1000);
}
