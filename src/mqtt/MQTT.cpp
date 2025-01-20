#include <WiFi.h>
#include <PubSubClient.h>
#include <wifi/WiFiHandler.h>
#include "../bme280v3/BME280.h"


const char* ssid = "Hotspot";
const char* password = "heineken123";


const char* mqtt_server = "192.168.196.156";
const int mqtt_port = 1885;
const char* mqtt_user = "user2";
const char* mqtt_password = "password";

const char* user_id = "2";
const char* device_id = "device_1";

BME280 bme280;

WiFiClient espClient;
PubSubClient client(espClient);
WiFiHandler wifiHandler(ssid, password);


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

// String generate_payload(){
//   float temperature = 25.0 + random(-100, 100) / 100.0;
//   float air_quality = 50.0 + random(-500, 500) / 100.0;
//   float pressure = 1013.0 + random(-200, 200) / 100.0;
//   float humidity = 60.0 + random(-300, 300) / 100.0;
//   String payload = "{";
//   payload += "\"temperature\":" + String(temperature) + ",";
//   payload += "\"air_quality\":" + String(air_quality) + ",";
//   payload += "\"pressure\":" + String(pressure) + ",";
//   payload += "\"humidity\":" + String(humidity);
//   payload += "}";
//   return payload;
// }

String generate_payload() {
    float temperature = bme280.readTemperature();
    float humidity = bme280.readHumidity();
    float pressure = bme280.readPressure() / 100.0F; // W hPa
    float air_quality = 50.0; // Tutaj możesz dodać inny czujnik lub stałą

    String payload = "{";
    payload += "\"temperature\":" + String(temperature, 2) + ",";
    payload += "\"humidity\":" + String(humidity, 2) + ",";
    payload += "\"pressure\":" + String(pressure, 2) + ",";
    payload += "\"air_quality\":" + String(air_quality, 2);
    payload += "}";

    return payload;
}


// void setup6() {
//   Serial.begin(9600);
//   wifiHandler.connectToWiFi();
//   client.setServer(mqtt_server, mqtt_port);
// }

void setup6() {
    Serial.begin(9600);
    wifiHandler.connectToWiFi();
    client.setServer(mqtt_server, mqtt_port);

    if (!bme280.begin(0x76)) {
        Serial.println("Nie znaleziono czujnika BME280!");
        while (1);
    }
    Serial.println("Czujnik BME280 zainicjalizowany.");
}


void loop6() {
  wifiHandler.monitorConnection();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temperature = 25.0 + random(-100, 100) / 100.0;
 
  String topic = String("/users/") + user_id + "/devices/" + device_id + "/data";
  String payload = generate_payload();
  client.publish(topic.c_str(), payload.c_str());
  Serial.print(payload);

  delay(5000);
}


