#include <BLEDevice.h>
BLEClient* pClient;

#define MJ_ADDR "A4:C1:38:8D:05:90"
static BLEAddress MJAddress(MJ_ADDR);
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
uint8_t* pData;
float temp = 0;
float humi = 0;
float volt = 0;


// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"); //
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6"); //

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {

  Serial.println("---------------------------------");
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);

  temp = (pData[0] | (pData[1] << 8)) * 0.01;
  Serial.println("Temp: " + String(temp));

  humi = pData[2];
  Serial.println("Humidity: " + String(humi));

  volt = ((pData[4] * 256) + pData[3]) / 1000.0;
  Serial.println("Voltage: " + String(volt));
  Serial.println("Battery Percentage: " + String(((volt - 2.1) * 100.0)));
  Serial.println("---------------------------------");


}



class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      Serial.println("on connect");
      connected = true;
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      Serial.println("onDisconnect");

    }
};

void getSensorData1() {

  Serial.println("Connecting to BLE Device");
  connectToServer(MJAddress);

}



void connectToServer(BLEAddress pAddress) {



  pClient->setClientCallbacks(new MyClientCallback());

  Serial.println("=================== Forming a connection to Mijia device ====================");
  Serial.println(pAddress.toString().c_str());
  Serial.println(" - Connection to Mijia Device");

  pClient->connect(pAddress);

  Serial.println(" - Connected to Mijia Device");


  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our characteristic");

  pRemoteCharacteristic->registerForNotify(notifyCallback);



}

void setup() {
  Serial.begin(115200);
  Serial.println("------ Starting ESP32 Bluetooth client... ------");


  BLEDevice::init("");
  delay(500);


  pClient  = BLEDevice::createClient();
  Serial.println("BLE Device - Client Created");
  delay(500);


  getSensorData1();


}

void loop() {


}
