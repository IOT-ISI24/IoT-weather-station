// #include "BLEDevice.h"
// #include "WiFi.h"
// #include "BLEScan.h"

// static BLEUUID serviceUUID("FFE0");
// static BLEUUID charUUID("FFE1");

// static boolean doConnect = false;
// static boolean connected = false;
// static boolean doScan = false;
// static BLERemoteCharacteristic *pRemoteCharacteristic;
// static BLEAdvertisedDevice *myDevice;

// static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
//   Serial.print("Notify callback for characteristic ");
//   Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//   Serial.print(" of data length ");
//   Serial.println(length);
//   Serial.print("data: ");
//   Serial.write(pData, length);
//   Serial.println();
// }

// class MyClientCallback : public BLEClientCallbacks {
//   void onConnect(BLEClient *pclient) {}

//   void onDisconnect(BLEClient *pclient) {
//     connected = false;
//     Serial.println("onDisconnect");
//   }
// };

// bool connectToServer() {
//   Serial.print("Forming a connection to ");
//   Serial.println(myDevice->getAddress().toString().c_str());

//   BLEClient *pClient = BLEDevice::createClient();
//   Serial.println(" - Created client");

//   pClient->setClientCallbacks(new MyClientCallback());


//   pClient->connect(myDevice); 
//   Serial.println(" - Connected to server");
//   pClient->setMTU(517); 


//   BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
//   if (pRemoteService == nullptr) {
//     Serial.print("Failed to find our service UUID: ");
//     Serial.println(serviceUUID.toString().c_str());
//     pClient->disconnect();
//     return false;
//   }
//   Serial.println(" - Found our service");


//   pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
//   if (pRemoteCharacteristic == nullptr) {
//     Serial.print("Failed to find our characteristic UUID: ");
//     Serial.println(charUUID.toString().c_str());
//     pClient->disconnect();
//     return false;
//   }
//   Serial.println(" - Found our characteristic");


//   if (pRemoteCharacteristic->canRead()) {
//     Serial.print("The characteristic value was: ");
//     Serial.println(1);
//   }

//   if (pRemoteCharacteristic->canNotify()) {
//     pRemoteCharacteristic->registerForNotify(notifyCallback);
//   }

//   connected = true;
//   return true;
// }

// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {

//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

//       BLEDevice::getScan()->stop();
//       myDevice = new BLEAdvertisedDevice(advertisedDevice);
//       doConnect = true;
//       doScan = true;

//     } 
//   } 
// }; 

// void setup5() {
//   Serial.begin(9600);
//   Serial.println("Starting Arduino BLE Client application...");
//   BLEDevice::init("");

//   BLEScan *pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setInterval(1349);
//   pBLEScan->setWindow(449);
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(5, false);
// }  


// void loop5() {

//   if (doConnect == true) {
//     if (connectToServer()) {
//       Serial.println("We are now connected to the BLE Server.");
//     } else {
//       Serial.println("We have failed to connect to the server; there is nothing more we will do.");
//     }
//     doConnect = false;
//   }

//     BLEDevice::getScan()->start(0); 

//   delay(1000);  
// }  