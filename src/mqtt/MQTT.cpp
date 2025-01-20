#include <WiFi.h>
#include <PubSubClient.h>
#include <wifi/WiFiHandler.h>
#include "../bme280v3/BME280.h"
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define LED_PIN 2 
#define DAC_PIN 25 
// const char* ssid = "hotspot";
// const char* password = "694202137";

const char* mqtt_server = "10.204.41.0";
const int mqtt_port = 1885;
const char* mqtt_user = "user2";
const char* mqtt_password = "password";

const char* user_id = "2";
const char* device_id = "device_1";

BME280 bme280;

WiFiClient espClient;
PubSubClient client(espClient);
// WiFiHandler wifiHandler(ssid, password);
WiFiHandler wifiHandler;


void reconnect() {
  while (!client.connected()) {
    Serial.print("Łączenie z brokerem MQTT...");
    if (client.connect(device_id, mqtt_user, mqtt_password)) {
      Serial.println("Połączono");
      String statusTopic = String("/users/") + user_id + "/devices/" + device_id + "/status";
      client.subscribe(statusTopic.c_str());
    } else {
      Serial.print("Błąd: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


String generate_payload() {
    float temperature = bme280.readTemperature();
    //float temperature = 50.0;
    //  float humidity = 50.0;

    float humidity = bme280.readHumidity();
    if (isnan(humidity)) {
      Serial.println("Błąd odczytu wilgotności!");
      return;
    }
    int dacValue = map(humidity, 0, 100, 0, 255);
    dacValue = constrain(dacValue, 0, 255); 
    dacWrite(DAC_PIN, dacValue);


     float pressure = 50.0;
     //float humidity = bme280.readHumidity();
     //float pressure = bme280.readPressure() / 100.0F; // W hPa
     float air_quality = 50.0; // Tutaj możesz dodać inny czujnik lub stałą

     String payload = "{";
     payload += "\"temperature\":" + String(temperature, 2) + ",";
     payload += "\"humidity\":" + String(humidity, 2) + ",";
     payload += "\"pressure\":" + String(pressure, 2) + ",";
     payload += "\"air_quality\":" + String(air_quality, 2);
     payload += "}";

     return payload;
 }


void setup() {
     Serial.begin(9600);
     Wire.begin(33, 32);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

     wifiHandler.connectToWiFi();
     Serial.println("I2C Scanner");
     client.setServer(mqtt_server, mqtt_port);

     for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
          Serial.print("Device found at address: 0x");
          Serial.println(addr, HEX);
        }
      }

     if (!bme280.begin(0x76, &Wire)) {
         Serial.println("Nie znaleziono czujnika BME280!");
         while (1);
     }
     Serial.println("Czujnik BME280 zainicjalizowany.");

     bme280.Meas_config(
        BME280::BME280_NORMAL_MODE,      // Normal mode
        BME280::BME280_FIlterMode_OFF,   // No filtering
        BME280::BME280_StandbyTime_MS_125, // 125ms standby time
        BME280::BME280_OVSX1,            // Pressure oversampling ×1
        BME280::BME280_OVSX1,            // Humidity oversampling ×1
        BME280::BME280_OVSX1             // Temperature oversampling ×1
    );
 }


void loop() {
  wifiHandler.monitorConnection();

  if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(LED_PIN, HIGH); // Turn on LED when connected to Wi-Fi
  } else {
      digitalWrite(LED_PIN, LOW); // Turn off LED when disconnected
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  String payload = generate_payload();
  String topic = String("/users/") + user_id + "/devices/" + device_id + "/data";
  client.publish(topic.c_str(), payload.c_str());
  delay(5000);
}


