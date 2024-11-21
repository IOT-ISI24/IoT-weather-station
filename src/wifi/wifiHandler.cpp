#include "WiFiHandler.h"

WiFiHandler::WiFiHandler(const char* ssid, const char* password)
    : ssid(ssid), password(password), host(host) {}

void WiFiHandler::connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssid, password);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isWiFiConnected = true;
    digitalWrite(ledPin, LOW);
}

void WiFiHandler::monitorConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        if (isWiFiConnected) {
            Serial.println("WiFi connection lost. Attempting to reconnect...");
            isWiFiConnected = false;
        }
        connectToWiFi();
    } else {
        if (!isWiFiConnected) {
            Serial.println("Reconnected to WiFi!");
            isWiFiConnected = true;
        }
    }
}
