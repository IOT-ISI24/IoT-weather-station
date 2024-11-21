#ifndef BLESERVERHANDLER_H
#define BLESERVERHANDLER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <random>

class BLEServerHandler {
public:
    BLEServerHandler(const std::string& deviceName, const std::string& serviceUUID, const std::string& characteristicUUID);
    void setup();
    void loop();

private:
    std::string deviceName;
    std::string serviceUUID;
    std::string characteristicUUID;
    BLECharacteristic *pCharacteristic;

    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> randint;

    void initializeBLE();
};

#endif // BLESERVERHANDLER_H
