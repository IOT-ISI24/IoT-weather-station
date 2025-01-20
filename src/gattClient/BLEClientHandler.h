// #ifndef BLECLIENTHANDLER_H
// #define BLECLIENTHANDLER_H

// #include <BLEDevice.h>
// #include <WiFi.h>
// #include <BLEScan.h>

// class BLEClientHandler {
// public:
//     BLEClientHandler(const BLEUUID& serviceUUID, const BLEUUID& charUUID);
//     void setup();
//     void loop();
    

// private:
//     BLEUUID serviceUUID;
//     BLEUUID charUUID;
//     bool connected;
//     BLERemoteCharacteristic *pRemoteCharacteristic;
//     BLEAdvertisedDevice *myDevice;

//     class MyClientCallback : public BLEClientCallbacks {
//     BLEClientHandler* handler;
//     public:
//     MyClientCallback(BLEClientHandler* handler) : handler(handler) {}
//     void onConnect(BLEClient *pclient) override;
//     void onDisconnect(BLEClient *pclient) override;
// };

//     class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
//         BLEClientHandler* handler;
//     public:
//         MyAdvertisedDeviceCallbacks(BLEClientHandler* handler);
//         void onResult(BLEAdvertisedDevice advertisedDevice) override;
//     };
    
//     static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);
//     bool connectToServer();
// };

// #endif // BLECLIENTHANDLER_H
    