#include "WiFiHandler.h"
#include "ArduinoNvs.h"

void clearAllNVS() {
    NVS.begin(); // Initialize NVS
    NVS.eraseAll(); // Erase all stored data
    NVS.commit(); // Commit the changes
    Serial.println("All NVS data cleared.");
}

// WiFiHandler::WiFiHandler(const char* ssid, const char* password)
//     : ssid(ssid), password(password), host(host) {}

WiFiHandler::WiFiHandler() : isWiFiConnected(false) {}


void WiFiHandler::setupProvisioning() {
    Serial.println("Initializing BLE provisioning...");
    // Clear existing credentials and start BLE provisioning
    NVS.begin();
    NVS.erase("net_ssid", false);
    NVS.erase("net_pass", false);
    NVS.commit();

    Serial.println("Starting BLE provisioning...");
    WiFiProv.beginProvision(
        WIFI_PROV_SCHEME_BLE,
        WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
        WIFI_PROV_SECURITY_1,
        "123qwe456",               // BLE provisioning PIN
        "PROV_ESP32_WEATHER_STATION" // BLE service name
    );
}


void WiFiHandler::connectToWiFi() {

    Serial.println("Checking for Wi-Fi credentials in the ConnectToWiFi function...");

    NVS.begin();

    // Check if credentials exist in NVS
    String net_ssid = NVS.getString("net_ssid");
    String net_pass = NVS.getString("net_pass");

    if (net_ssid.isEmpty() || net_pass.isEmpty()) {
        Serial.println("No Wi-Fi credentials found. Entering provisioning mode...");
        setupProvisioning();
        return;
    }

        // Connect to Wi-Fi
    Serial.println("Connecting to Wi-Fi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(net_ssid.c_str(), net_pass.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isWiFiConnected = true;
}

void WiFiHandler::monitorConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected. Reconnecting...");
        connectToWiFi();
    }
}


//     Serial.print("Connecting to ");
//     Serial.println(ssid);

//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
//     delay(100);

//     WiFi.begin(ssid, password);
//     Serial.println("Connecting...");

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.print(".");
//     }

//     Serial.println("\nWiFi connected");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
//     isWiFiConnected = true;
//     digitalWrite(ledPin, LOW);
// }



// void WiFiHandler::monitorConnection() {
//     if (WiFi.status() != WL_CONNECTED) {
//         if (isWiFiConnected) {
//             Serial.println("WiFi connection lost. Attempting to reconnect...");
//             isWiFiConnected = false;
//         }
//         connectToWiFi();
//     } else {
//         if (!isWiFiConnected) {
//             Serial.println("Reconnected to WiFi!");
//             isWiFiConnected = true;
//         }
//     }
// }
