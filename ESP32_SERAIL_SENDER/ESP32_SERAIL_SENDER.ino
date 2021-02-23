#include <HardwareSerial.h>
HardwareSerial UART0(2);

void setup() {
  Serial.begin(115200);
  UART0.begin(115200, SERIAL_8N1, 17, 16); //rx2,tx2
  Serial.println("Sender.............");
}

void loop() {
  // // read serial
  // while (UART0.available()) {
  //   Serial.write(UART0.read());
  // }
  // // read monitor
  // while (Serial.available()) {
  //   UART0.write(Serial.read());
  // }
  String text = "/"+String("value1")+"/"+String("value2");
  UART0.println(text);
  delay(1000);
}
