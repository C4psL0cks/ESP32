#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2 , TXD2);

}

void loop() {

  if (Serial2.available() > 0) {
    Serial.println("RECEIVE: " + String(Serial2.readString()));
  }
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    Serial.println("SEND: " + String(msg));
    Serial2.print(msg);
  }

}
