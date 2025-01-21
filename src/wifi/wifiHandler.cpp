#include "WiFiHandler.h"

constexpr const char* ble_pop = "123qwe456"; // BLE provisioning PIN
constexpr const char* ble_service = "PROV_ESP32_WEATHER_STATION"; // BLE service name

WiFiHandler::WiFiHandler() : isWiFiConnected(false) {}

// Starts BLE provisioning
void WiFiHandler::setupProvisioning() {
    Serial.println("Starting BLE provisioning...");
    NVS.begin();
    NVS.erase("net_ssid", false); // Clear any existing credentials
    NVS.erase("net_pass", false);
    NVS.commit();

    WiFiProv.beginProvision(
        WIFI_PROV_SCHEME_BLE,
        WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
        WIFI_PROV_SECURITY_1,
        ble_pop,
        ble_service
    );
}

// Attempts to connect to Wi-Fi using stored credentials
void WiFiHandler::connectToWiFiUsingSavedCredentials() {
    NVS.begin();

    NVS.erase("net_ssid", false); // Clear any existing credentials
    NVS.erase("net_pass", false);


    String net_ssid = NVS.getString("net_ssid");
    String net_pass = NVS.getString("net_pass");

    if (net_ssid.isEmpty() || net_pass.isEmpty()) {
        Serial.println("No saved Wi-Fi credentials found. Entering provisioning mode...");
        setupProvisioning();
        return;
    }

    Serial.println("Connecting to Wi-Fi with saved credentials...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(net_ssid.c_str(), net_pass.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isWiFiConnected = true;
}

// Handles the Wi-Fi connection
void WiFiHandler::connectToWiFi() {
    WiFi.onEvent([](arduino_event_t* ev) {
        switch (ev->event_id) {
            case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                Serial.print("Connected: ");
                Serial.println(IPAddress(ev->event_info.got_ip.ip_info.ip.addr));
                break;
            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                Serial.println("Wi-Fi disconnected. Reconnecting...");
                WiFi.begin();
                break;
            case ARDUINO_EVENT_PROV_START:
                Serial.println("Provisioning started. Use ESP BLE Prov app to set Wi-Fi credentials.");
                break;
            case ARDUINO_EVENT_PROV_CRED_RECV:
                Serial.println("Credentials received!");
                break;
            case ARDUINO_EVENT_PROV_CRED_SUCCESS:
                Serial.println("Provisioning successful. Connecting to Wi-Fi...");
                break;
            case ARDUINO_EVENT_PROV_CRED_FAIL:
                Serial.println("Provisioning failed.");
                break;
            case ARDUINO_EVENT_PROV_END:
                Serial.println("Provisioning process ended.");
                break;
            default:
                break;
        }
    });

    connectToWiFiUsingSavedCredentials();
}

// Monitors the Wi-Fi connection and reconnects if necessary
void WiFiHandler::monitorConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected. Reconnecting...");
        connectToWiFiUsingSavedCredentials();
    }
}




// #include "WiFiHandler.h"

// WiFiHandler::WiFiHandler(const char* ssid, const char* password)
//     : ssid(ssid), password(password), host(host) {}

// void WiFiHandler::connectToWiFi() {
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
