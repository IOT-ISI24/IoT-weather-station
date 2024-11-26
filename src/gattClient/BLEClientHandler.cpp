#include "BLEClientHandler.h"

BLEClientHandler::BLEClientHandler(const BLEUUID& serviceUUID, const BLEUUID& charUUID)
    : serviceUUID(serviceUUID), charUUID(charUUID), connected(false),
      pRemoteCharacteristic(nullptr), myDevice(nullptr) {}

void BLEClientHandler::setup() {
    Serial.begin(9600);
    Serial.println("Starting Arduino BLE Client application...");
    BLEDevice::init("");

    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pBLEScan->setInterval(1349); //how often
    pBLEScan->setWindow(449); // how long
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void BLEClientHandler::loop() {
    if (!connected) {
        if (connectToServer()) {
            Serial.println("We are now connected to the BLE Server.");
        } else {
            Serial.println("Failed to connect to the server.");
        }
        connected = true;
    }
    BLEDevice::getScan()->start(0);
    delay(1000);
}

bool BLEClientHandler::connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback(this));
    pClient->connect(myDevice);
    Serial.println(" - Connected to server");

    pClient->setMTU(30); // min 23 bytes

    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.println("Failed to find our service UUID.");
        pClient->disconnect();
        return false;
    }

    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
        Serial.println("Failed to find our characteristic UUID.");
        pClient->disconnect();
        return false;
    }

    if (pRemoteCharacteristic->canNotify()) {
        pRemoteCharacteristic->registerForNotify(notifyCallback);
    }

    connected = true;
    return true;
}

void BLEClientHandler::MyClientCallback::onConnect(BLEClient *pclient) {}

void BLEClientHandler::MyClientCallback::onDisconnect(BLEClient *pclient) {
    this->handler->connected = false;
    Serial.println("Disconnected from server.");
}

BLEClientHandler::MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(BLEClientHandler* handler) : handler(handler) {}

void BLEClientHandler::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(handler->serviceUUID)) {
        BLEDevice::getScan()->stop();
        handler->myDevice = new BLEAdvertisedDevice(advertisedDevice);
    }
}

void BLEClientHandler::notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print("Data: ");
    Serial.write(pData, length);
    Serial.println();
}
