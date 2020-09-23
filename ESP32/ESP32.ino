//void setup() {
//  Serial.begin(115200);
//}
//void loop() {
//  Serial.println("HELLO WORLD");
//  delay(1000);
//}

#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"
//#include "DHT.h"

INTERNET net;
FIREBASE firebase;
//SIM TRUE internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define DHTPIN 2
#define DHTTYPE DHT22
#define LED 10
//DHT dht(DHTPIN, DHTTYPE);
// Config Firebase
#define FIREBASE_URL "gsms-1efb6.firebaseio.com"
#define FIREBASE_SECRET "U0gghGOrOZzsYOdL7t8uwTgMGwErUjlfymGgqoGr"

unsigned long previousMillis = 0;
const long interval = 2000;

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}
void setup()
{
  pinMode(LED, OUTPUT);       //ให้ LED (Pin 10) เป็น Output
  digitalWrite(LED, HIGH);    //สั่งให้จ่าย Logic High (LED ดับ)
  //  dht.begin();          //เริ่มใช้งาน DHT
  Serial.begin(9600);       //เริ่มใช้งาน Serial
  gsm.begin(&mySerial, 9600);     //กำหนดช่องทาง Serial ที่ต่อกับ 3GShield
  gsm.Event_debug = debug;      //กำหนด Function ที่ใช้แสดง Debug
  Serial.println(F("UC20"));
  gsm.PowerOn();          //สั่งงานปิด และ เปิด 3G Module
  while (gsm.WaitReady()) {}    //รอจนกว่า 3G จะเชื่อมต่อกับเครือข่ายและพร้อมใช้งาน
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());  //แสดงชื่อเครือข่ายที่ใช้
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());  //แสดงความแรงของสัญญาณ
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS); //ตั้งค่า APN USER PASSWORD
  Serial.println(F("Connect net"));
  net.Connect();          //เชื่อมต่อ Internet
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());    //แสดง IP
  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  //ตั้งค่า url และ secret ของ firebase database
  int ret = firebase.connect(); //เชื่อมต่อกับ Firebase
  if (ret == 1)     //ถ้าหากเชื่อมต่อสำเร็จ
  {
    Serial.println("Remove Object LED");
    firebase.remove("LED");   //ลบ Object ที่ชื่อ "LED" ทิ้ง
    Serial.println("Create Object LED");
    firebase.setInt("LED", 0); //สร้าง Object ที่ชื่อ "LED" และกำหนด value = 0
  }
  firebase.close();      //ปิดการเชื่อมต่อกับ Firebase
  previousMillis =  millis();  //กำหนดเวลาปัจจุบัน ให้เป็นค่าเวลาก่อนหน้า
}
void loop()
{
  unsigned long currentMillis = millis(); //อ่านค่าเวลาปัจจุบัน

  //ถ้าหากเวลาปัจจุบันลบกับเวลาก่อนหน้า มากกว่าหรือเท่ากับค่าเวลาในตัวแปล Interval (ทำงานทุกๆ 2 วินาที)
  if (currentMillis - previousMillis >= interval)
  {
    int ret = firebase.connect();    //เชื่อมต่อกับ Filebase
    if (ret == 1)
    {
      float h = random(300);   //อ่านค่าความชื้นจาก Sensor
      float t = random(300); //อ่านค่าอุณหภูมิจาก Sensor
      Serial.println("Set Temperature = " + String(t));
      firebase.setFloat("Temperature", t); //ส่งค่าอุณหภูมิไปยัง Topic "Temperature"
      Serial.println("Set Humidity = " + String(t));
      firebase.setFloat("Humidity", h); //ส่งค่าความชื้นไปยัง Topic "Humidity"
      int led = firebase.getInt("LED"); //อ่านค่าจาก Object "LED"
      Serial.println("Get LED = " + String(led));
      if (led == 1)   //ถ้าหาก value ใน Object "LED" เท่ากับ 1 ให้ LED ติดถ้าไม่ใช่ให้ดับ
        digitalWrite(LED, LOW);
      else
        digitalWrite(LED, HIGH);
    }
    firebase.close();       //ปิดการเชื่อมต่อกับ Filebase
    previousMillis = currentMillis;
  }
}
