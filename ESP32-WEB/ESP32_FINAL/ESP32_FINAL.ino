#include <WiFiManager.h>
#include <WiFi.h>
#define RXD2 16
#define TXD2 17
WiFiServer server(80);

// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String modes = "off";
String outputled1State = "off";
String outputled2State = "off";
String outputled3State = "off";
String outputfan1State = "off";
String outputfan2State = "off";
String outputfan3State = "off";
String outputpumpState = "off";
String outputsvState = "off";

// Assign output variables to GPIO pins

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2 , TXD2);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //  wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("ESP32", "1234567890")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  server.begin();

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //MODE
            if (header.indexOf("GET /mode/on") >= 0) {
              Serial.println("GPIO mode on");
              modes = "on";
              Serial2.print(90);

            } else if (header.indexOf("GET /mode/off") >= 0) {
              Serial.println("GPIO mode off");
              modes = "off";
              Serial2.print(91);

            }

            // LED
            else if (header.indexOf("GET /led1/on") >= 0) {
              Serial.println("GPIO led1 on");
              outputled1State = "on";
              Serial2.print(11);

            } else if (header.indexOf("GET /led1/off") >= 0) {
              Serial.println("GPIO led1 off");
              outputled1State = "off";
              Serial2.print(10);

            } else if (header.indexOf("GET /led2/on") >= 0) {
              Serial.println("GPIO led2 on");
              outputled2State = "on";
              Serial2.print(21);

            } else if (header.indexOf("GET /led2/off") >= 0) {
              Serial.println("GPIO led2 off");
              outputled2State = "off";
              Serial2.print(20);

            } else if (header.indexOf("GET /led3/on") >= 0) {
              Serial.println("GPIO led3 on");
              outputled3State = "on";
              Serial2.print(31);

            } else if (header.indexOf("GET /led3/off") >= 0) {
              Serial.println("GPIO led3 off");
              outputled3State = "off";
              Serial2.print(30);
            }

            // FAN
            else if (header.indexOf("GET /fan1/on") >= 0) {
              Serial.println("GPIO FAN1 on");
              outputfan1State = "on";
              Serial2.print(41);

            } else if (header.indexOf("GET /fan1/off") >= 0) {
              Serial.println("GPIO FAN1 off");
              outputfan1State = "off";
              Serial2.print(40);

            } else if (header.indexOf("GET /fan2/on") >= 0) {
              Serial.println("GPIO FAN2 on");
              outputfan2State = "on";
              Serial2.print(51);

            } else if (header.indexOf("GET /fan2/off") >= 0) {
              Serial.println("GPIO FAN2 off");
              outputfan2State = "off";
              Serial2.print(50);

            } else if (header.indexOf("GET /fan3/on") >= 0) {
              Serial.println("GPIO FAN3 on");
              outputfan3State = "on";
              Serial2.print(61);

            } else if (header.indexOf("GET /fan3/off") >= 0) {
              Serial.println("GPIO FAN3 off");
              outputfan3State = "off";
              Serial2.print(60);
            }

            // PUMP
            else if (header.indexOf("GET /pump/on") >= 0) {
              Serial.println("GPIO pump on");
              outputpumpState = "on";
              Serial2.print(71);

            } else if (header.indexOf("GET /pump/off") >= 0) {
              Serial.println("GPIO pump off");
              outputpumpState = "off";
              Serial2.print(70);
            }

            // SV
            else if (header.indexOf("GET /sv/on") >= 0) {
              Serial.println("GPIO sv on");
              outputsvState = "on";
              Serial2.print(81);

            } else if (header.indexOf("GET /sv/off") >= 0) {
              Serial.println("GPIO sv off");
              outputsvState = "off";
              Serial2.print(80);
            }


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");


            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO modes - State " + modes + "</p>");
            // If the output26State is off, it displays the ON button
            if (modes == "off") {
              client.println("<p><a href=\"/mode/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/mode/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO led1 - State " + outputled1State + "</p>");
            // If the output26State is off, it displays the ON button
            if (outputled1State == "off") {
              client.println("<p><a href=\"/led1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/led1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO led2 - State " + outputled2State + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputled2State == "off") {
              client.println("<p><a href=\"/led2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/led2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO led3 - State " + outputled3State + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputled3State == "off") {
              client.println("<p><a href=\"/led3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/led3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO fan1 - State " + outputfan1State + "</p>");
            // If the output26State is off, it displays the ON button
            if (outputfan1State == "off") {
              client.println("<p><a href=\"/fan1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/fan1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO fan2 - State " + outputfan2State + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputfan2State == "off") {
              client.println("<p><a href=\"/fan2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/fan2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO fan3 - State " + outputfan3State + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputfan3State == "off") {
              client.println("<p><a href=\"/fan3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/fan3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO pump - State " + outputpumpState + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputpumpState == "off") {
              client.println("<p><a href=\"/pump/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/pump/off\"><button class=\"button button2\">OFF</button></a></p>");
            }



            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO sv - State " + outputsvState + "</p>");
            // If the output27State is off, it displays the ON button
            if (outputsvState == "off") {
              client.println("<p><a href=\"/sv/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/sv/off\"><button class=\"button button2\">OFF</button></a></p>");
            }



            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}
