#include "BLEClientHandler.h"

BLEUUID serviceUUID("FFE0");
BLEUUID charUUID("FFE1");

BLEClientHandler bleClientHandler(serviceUUID, charUUID);

void setup12() {
    bleClientHandler.setup();
}

void loop12() {
    bleClientHandler.loop();
}
