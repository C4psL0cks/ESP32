#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "dlink-ABD0";
const char* password = "yyafr68490";


String pwmSliderValue1 = "0";
String pwmSliderValue2 = "0";
String pwmSliderValue3 = "0";
String pwmSliderValue4 = "0";

// Set Desired PWM Settings
const int frequencyHz = 5000;
const int pwmChannel = 0;
const int resolution = 8;

const char* INPUT_PARAMETER1 = "value";
const char* INPUT_PARAMETER2 = "value";
const char* INPUT_PARAMETER3 = "value";
const char* INPUT_PARAMETER4 = "value";

AsyncWebServer webServer(80);

const char htmlCode[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <!  define meta data >
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <! define the style CSS of your page >
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h1 {font-size: 2.9rem;}
    h2 {font-size: 2.1rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 30px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 400px; height: 15px; border-radius: 5px; background: #39a6de; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 12px; background: #f74d4d; cursor: pointer;}
    .slider::-moz-range-thumb { width: 25px; height: 25px; border-radius: 12px; background: #F74D4D; cursor: pointer; }
  </style>
</head>
<body>
  <! Edit the pages your heading 1 >
  <h2>ESP32 PWM Slider</h2>
  <h2>LED 1 Brightness</h2>
  <! Displays the value of the slider >
  <p><span id="textSliderValue1">%SLIDERVALUE1%</span> &#37</p>
  <! displays the range of the slider 0 - 100 in steps of 1 >
  <p><input type="range" onchange="updateSliderPWM1(this)" id="pwmSlider1" min="0" max="100" value="%SLIDERVALUE1%" step="1" class="slider"></p>
  <br>
  <h2>LED 2 Brightness</h2>
  <! Displays the value of the slider >
  <p><span id="textSliderValue2">%SLIDERVALUE2%</span> &#37</p>
  <! displays the range of the slider 0 - 100 in steps of 1 >
  <p><input type="range" onchange="updateSliderPWM2(this)" id="pwmSlider2" min="0" max="100" value="%SLIDERVALUE2%" step="1" class="slider"></p>
  <br>
  <h2>LED 3 Brightness</h2>
  <! Displays the value of the slider >
  <p><span id="textSliderValue3">%SLIDERVALUE3%</span> &#37</p>
  <! displays the range of the slider 0 - 100 in steps of 1 >
  <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="100" value="%SLIDERVALUE3%" step="1" class="slider"></p>
  <br>
  <h2>LED 4 Brightness</h2>
  <! Displays the value of the slider >
  <p><span id="textSliderValue4">%SLIDERVALUE4%</span> &#37</p>
  <! displays the range of the slider 0 - 100 in steps of 1 >
  <p><input type="range" onchange="updateSliderPWM4(this)" id="pwmSlider4" min="0" max="100" value="%SLIDERVALUE4%" step="1" class="slider"></p>
  <br>
<script>
function updateSliderPWM1(element) {

  var pwmSliderValue1 = document.getElementById("pwmSlider1").value;
  document.getElementById("textSliderValue1").innerHTML = pwmSliderValue1;
  console.log(pwmSliderValue1);

  var httpRequest = new XMLHttpRequest();
  httpRequest.open("GET", "/slider1?value="+pwmSliderValue1, true);
  httpRequest.send();
}
function updateSliderPWM2(element) {

  var pwmSliderValue2 = document.getElementById("pwmSlider2").value;
  document.getElementById("textSliderValue2").innerHTML = pwmSliderValue2;
  console.log(pwmSliderValue2);

  var httpRequest = new XMLHttpRequest();
  httpRequest.open("GET", "/slider2?value="+pwmSliderValue2, true);
  httpRequest.send();
}

function updateSliderPWM3(element) {

  var pwmSliderValue3 = document.getElementById("pwmSlider3").value;
  document.getElementById("textSliderValue3").innerHTML = pwmSliderValue3;
  console.log(pwmSliderValue3);

  var httpRequest = new XMLHttpRequest();
  httpRequest.open("GET", "/slider3?value="+pwmSliderValue3, true);
  httpRequest.send();
}

function updateSliderPWM4(element) {

  var pwmSliderValue4 = document.getElementById("pwmSlider4").value;
  document.getElementById("textSliderValue4").innerHTML = pwmSliderValue4;
  console.log(pwmSliderValue4);

  var httpRequest = new XMLHttpRequest();
  httpRequest.open("GET", "/slider4?value="+pwmSliderValue4, true);
  httpRequest.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces the placeholder with the button in your web page
String updateButton1(const String& var) {
  if (var == "SLIDERVALUE1") {
    return pwmSliderValue1;
  }
  return String();
}
String updateButton2(const String& var) {
  if (var == "SLIDERVALUE2") {
    return pwmSliderValue2;
  }
  return String();
}

String updateButton3(const String& var) {
  if (var == "SLIDERVALUE3") {
    return pwmSliderValue3;
  }
  return String();
}
String updateButton4(const String& var) {
  if (var == "SLIDERVALUE4") {
    return pwmSliderValue4;
  }
  return String();
}

void setup() {
  // Begine Serial Communications over USB
  Serial.begin(115200);

  ledcSetup(pwmChannel, frequencyHz, resolution);
  ledcAttachPin(4, pwmChannel);
//  ledcWrite(pwmChannel, pwmSliderValue1.toInt());

  // Connect to your Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print the IP Address of your device
  Serial.println(WiFi.localIP());

  // Detail the route for root / web page
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", htmlCode, updateButton1);
  });

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", htmlCode, updateButton2);
  });

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", htmlCode, updateButton3);
  });

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", htmlCode, updateButton4);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  webServer.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;

    if (request->hasParam(INPUT_PARAMETER1)) {
      inputMessage1 = request->getParam(INPUT_PARAMETER1)->value();
      pwmSliderValue1 = inputMessage1;
      ledcWrite(pwmChannel, pwmSliderValue1.toInt());
    }
    else {
      inputMessage1 = "No message sent";
    }
    Serial.println("M1:"+inputMessage1);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  webServer.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage2;

    if (request->hasParam(INPUT_PARAMETER2)) {
      inputMessage2 = request->getParam(INPUT_PARAMETER2)->value();
      pwmSliderValue2 = inputMessage2;
      ledcWrite(pwmChannel, pwmSliderValue2.toInt());
    }
    else {
      inputMessage2 = "No message sent";
    }
    Serial.println("M2:"+inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  webServer.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage3;

    if (request->hasParam(INPUT_PARAMETER3)) {
      inputMessage3 = request->getParam(INPUT_PARAMETER3)->value();
      pwmSliderValue3 = inputMessage3;
      ledcWrite(pwmChannel, pwmSliderValue3.toInt());
    }
    else {
      inputMessage3 = "No message sent";
    }
    Serial.println("M3:"+inputMessage3);
    request->send(200, "text/plain", "OK");
  });


  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  webServer.on("/slider4", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage4;

    if (request->hasParam(INPUT_PARAMETER4)) {
      inputMessage4 = request->getParam(INPUT_PARAMETER4)->value();
      pwmSliderValue4 = inputMessage4;
      ledcWrite(pwmChannel, pwmSliderValue2.toInt());
    }
    else {
      inputMessage4 = "No message sent";
    }
    Serial.println("M4:"+inputMessage4);
    request->send(200, "text/plain", "OK");
  });

  // Start server (remembering its on port 80)
  webServer.begin();
}

void loop() {

}
