#include "BLEServerHandler.h"
#include <Arduino.h>

BLEServerHandler::BLEServerHandler(const std::string& deviceName, const std::string& serviceUUID, const std::string& characteristicUUID)
    : deviceName(deviceName), serviceUUID(serviceUUID), characteristicUUID(characteristicUUID),
      rng(dev()), randint(1, 10), pCharacteristic(nullptr) {}

void BLEServerHandler::setup() {
    Serial.begin(9600);
    Serial.println("Starting BLE server...");

    initializeBLE();
}

void BLEServerHandler::loop() {
    delay(2000);
    int randomValue = randint(rng);
    Serial.print("Generated random value: ");
    Serial.println(randomValue);
    pCharacteristic->setValue(String(randomValue).c_str());
}

void BLEServerHandler::initializeBLE() {
    BLEDevice::init(deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(serviceUUID);

    pCharacteristic = pService->createCharacteristic(
        characteristicUUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setValue("Initial");
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    BLEDevice::startAdvertising();
    Serial.println("BLE server initialized and advertising started!");
}
