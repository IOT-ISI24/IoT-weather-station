#include "BLEServerHandler.h"

#define SERVICE_UUID "FFE2"
#define CHARACTERISTIC_UUID "FFE3"

BLEServerHandler bleServer("BLE server", SERVICE_UUID, CHARACTERISTIC_UUID);

void setup1() {
    bleServer.setup();
}

void loop1() {
    bleServer.loop();
}
