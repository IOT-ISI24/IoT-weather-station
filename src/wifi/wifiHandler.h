#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiProv.h>
#include "ArduinoNvs.h"

class WiFiHandler {
private:
    // const char* ssid;
    // const char* password;
    String ssid;
    String password;
    const char* host;
    int port;
    int ledPin;
    bool isWiFiConnected;

    void setupProvisioning();

public:
    // WiFiHandler(const char* ssid, const char* password);
    WiFiHandler();

    void connectToWiFi();
    void monitorConnection();
};

#endif
