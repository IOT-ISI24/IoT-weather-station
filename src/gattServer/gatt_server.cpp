#include "BLEServerHandler.h"

#define SERVICE_UUID "FFE2"
#define CHARACTERISTIC_UUID "FFE3"

BLEServerHandler bleServer("BLE server", SERVICE_UUID, CHARACTERISTIC_UUID);

void setup() {
    bleServer.setup();
}

void loop() {
    bleServer.loop();
}
