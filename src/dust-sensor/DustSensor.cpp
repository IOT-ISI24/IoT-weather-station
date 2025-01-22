#include "DustSensor.h"

DustSensor::DustSensor(uint8_t analogPin, uint8_t ledPin)
    : _analogPin(analogPin), _ledPin(ledPin) {}

void DustSensor::begin() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW); 
}

float DustSensor::readDustDensity() {
    digitalWrite(_ledPin, HIGH);
    delayMicroseconds(280); 

    int rawValue = analogRead(_analogPin);
    float voltage = rawValue * adcResolution;

    digitalWrite(_ledPin, LOW);

    float dustDensity = (voltage / sensitivity) * 0.1; 
    return dustDensity;
}
