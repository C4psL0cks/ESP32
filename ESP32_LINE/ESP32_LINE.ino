#include <WiFi.h>
#include <TridentTD_LineNotify.h>

// Config connect WiFi
#define WIFI_SSID "dlink-ABD0"
#define WIFI_PASSWORD "yyafr68490"
#define LINE_TOKEN  "VOwca6uoY6T5vR6A43J7zwg2PtOWG2z4wGvgTLFsN3u"

void setup() {

  Serial.begin(115200);
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

  LINE.setToken(LINE_TOKEN);

  LINE.notify("อุณหภูมิ เกินกำหนด");


}
void loop() {

}
