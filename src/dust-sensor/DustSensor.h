#ifndef DUST_SENSOR_H
#define DUST_SENSOR_H

#include <Arduino.h>

class DustSensor {
public:
    DustSensor(uint8_t analogPin, uint8_t ledPin);
    void begin();
    float readDustDensity();

private:
    uint8_t _analogPin;
    uint8_t _ledPin;
    const float sensitivity = 0.5; 
    const float adcResolution = 3.3 / 4095.0; 
};

#endif
