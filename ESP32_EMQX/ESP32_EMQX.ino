#include <WiFi.h>
#include <PubSubClient.h>

#define WIFI_STA_NAME "dlink-ABD0"
#define WIFI_STA_PASS "yyafr68490"

#define MQTT_SERVER     "broker.emqx.io"
#define MQTT_PORT       1883
#define MQTT_USERNAME   "test"
#define MQTT_PASSWORD   "test"
#define MQTT_NAME       "mqttx_adbf3ddb"
#define TOPIC           "esp32/test"

#define LED_PIN 23

WiFiClient client;
PubSubClient mqtt(client);

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char*)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);
  if (payload_str == "0") {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  //  digitalWrite(LED_PIN, (payload_str == "ON") ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}

void loop() {
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe(TOPIC);
      mqtt.setCallback(callback);
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
  }
}
