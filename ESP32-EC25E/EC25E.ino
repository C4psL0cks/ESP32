#include <HardwareSerial.h>

#define RXPIN  14
#define TXPIN  27

HardwareSerial UART0(2); 

void setup() {
  Serial.begin(115200);
  UART0.begin(115200, SERIAL_8N1, RXPIN, TXPIN);
  Serial.println("START.............");
}

void loop() {
  // read serial
  while (UART0.available()) {
    Serial.write(UART0.read());
  }
  // read monitor
  while (Serial.available()) {
    UART0.write(Serial.read());
  }
}
