#include "DustSensor.h"

DustSensor::DustSensor(uint8_t analogPin, uint8_t ledPin) 
    : _analogPin(analogPin), _ledPin(ledPin) {}

void DustSensor::begin() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW); // Turn off LED by default
}

float DustSensor::readDustDensity() {
    // Turn on the LED to enable measurement
    digitalWrite(_ledPin, HIGH);
    delayMicroseconds(280); // Wait for sensor to stabilize

    // Read analog value
    int rawValue = analogRead(_analogPin);
    float voltage = rawValue * adcResolution;

    // Turn off the LED
    digitalWrite(_ledPin, LOW);

    // Convert voltage to dust density in mg/m³
    float dustDensity = (voltage / sensitivity) * 0.1; // mg/m³
    return dustDensity;
}
