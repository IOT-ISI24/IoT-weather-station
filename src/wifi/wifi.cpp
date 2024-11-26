    #include <Arduino.h>
    #include "WiFi.h"


const char* WIFI_SSID = "motorola edge 30 neo_9883";
const char* WIFI_PASS = "motorola";

const char* HOST = "neverssl.com";
const int PORT = 80;

const int LED_PIN = 2; 
bool isWiFiConnected = false; 

void connectToWiFi() {;
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isWiFiConnected = true;
    digitalWrite(LED_PIN, LOW); 

}

void fetchWebPage() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;

        Serial.print("Connecting to ");
        Serial.print(HOST);
        Serial.print(":");
        Serial.println(PORT);

        if (!client.connect(HOST, PORT)) {
            Serial.println("Connection to server failed!");
            return;
        }

        client.println("GET / HTTP/1.1");
        client.print("Host: ");
        client.println(HOST);
        client.println("Connection: close");
        client.println();

        Serial.println("Response:");
        while (client.connected() || client.available()) {
            if (client.available()) {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }

        client.stop(); 
        Serial.println("Connection closed.");
    } else {
        Serial.println("WiFi not connected. Cannot fetch web page.");
    }
}

void setup1233() {
    Serial.begin(9600);
    delay(2000);

    Serial.println("\nRunning Firmware.");
    connectToWiFi();
    fetchWebPage(); 
}

void loop1233() {
    if (WiFi.status() != WL_CONNECTED) {
        if (isWiFiConnected) {
            Serial.println("ups! WiFi connection lost."); 
            isWiFiConnected = false;
            digitalWrite(LED_PIN, HIGH);
        }
        connectToWiFi(); 
    } else {
        if (!isWiFiConnected) {
            Serial.println("Reconnected to WiFi!");
            isWiFiConnected = true;
            fetchWebPage(); 
            digitalWrite(LED_PIN, LOW);
        }
    }

    delay(1000); 
}



// #include <Arduino.h>
// #include "WiFi.h"

// // WiFi credentials
// const char* WIFI_SSID = "motorola edge 30 neo_9883";
// const char* WIFI_PASS = "motorola";

// const int LED_PIN = 2; // Pin GPIO2 dla diody LED
// bool isWiFiConnected = false; // Zmienna oznaczająca stan połączenia WiFi

// void setup() {
//     Serial.begin(9600);
//     pinMode(LED_PIN, OUTPUT); // Ustawienie pinu diody jako wyjścia
//     digitalWrite(LED_PIN, LOW); // Ustawienie LED na wyłączoną

//     delay(2000);

//     Serial.println();
//     Serial.println("Running Firmware.");

//     // Połączenie z WiFi
//     Serial.println();
//     Serial.println();
//     Serial.print("Connecting to ");
//     Serial.println(WIFI_SSID);

//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
//     delay(100);

//     WiFi.begin(WIFI_SSID, WIFI_PASS);
//     Serial.println("Connecting...");

//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");  // Kropki dla wizualnego efektu
//         delay(500);
//     }

//     Serial.println("\nWiFi connected");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());
//     isWiFiConnected = true; // WiFi połączone, ustawiamy zmienną na true
// }

// void loop() {
//     // Sprawdzanie stanu połączenia WiFi co 500 ms
//     if (WiFi.status() != WL_CONNECTED) {
//         if (isWiFiConnected) {
//             Serial.println("WiFi connection lost. Attempting to reconnect...");
//             isWiFiConnected = false; // Zaktualizowanie stanu połączenia
//         }

//         // Mruganie diodą, jeśli brak połączenia
//         digitalWrite(LED_PIN, HIGH);
//         delay(200);
//         digitalWrite(LED_PIN, LOW);
//         delay(200);

//         // Próba ponownego połączenia co 2 sekundy
//         WiFi.disconnect();
//         WiFi.reconnect();
//         delay(2000);
//     } else {
//         // WiFi jest połączone, więc LED jest wyłączony
//         if (!isWiFiConnected) {
//             Serial.println("Reconnected to WiFi!");
//             isWiFiConnected = true;
//         }
//         digitalWrite(LED_PIN, LOW);
//         delay(500); // Opóźnienie 500 ms, gdy połączono
//     }
// }
