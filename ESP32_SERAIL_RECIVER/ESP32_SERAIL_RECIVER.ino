#include <HardwareSerial.h>
HardwareSerial UART0(2);

void setup() {
  Serial.begin(115200);
  UART0.begin(115200, SERIAL_8N1, 27, 14); //rx gpio,tx gpio
  Serial.println("Recive.............");
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

  String text =  UART0.readString();
  Serial.println(text);

}