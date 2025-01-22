

#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiProv.h>
#include "ArduinoNvs.h"

class WiFiHandler {
private:
    bool isWiFiConnected;

    void setupProvisioning();
    void connectToWiFiUsingSavedCredentials();

public:
    WiFiHandler();
    void connectToWiFi();
    void monitorConnection();
};

#endif




//////////////////////////////////////////////


// #ifndef WIFIHANDLER_H
// #define WIFIHANDLER_H

// #include <Arduino.h>
// #include <WiFi.h>

// class WiFiHandler {
// private:
//     const char* ssid;
//     const char* password;
//     const char* host;
//     int port;
//     int ledPin;
//     bool isWiFiConnected;

// public:
//     WiFiHandler(const char* ssid, const char* password);

//     void connectToWiFi();
//     void fetchWebPage();
//     void monitorConnection();
// };

// #endif
