#include <WiFi.h>
#include <PubSubClient.h>
#include <wifi/WiFiHandler.h>
#include "../bme280v3/BME280.h"
#include <Wire.h>

const char* ssid = "Hotspot";
const char* password = "iot_pres";
String mac = WiFi.macAddress();

const char* mqtt_server = "192.168.252.156";
const int mqtt_port = 1885;

int delay_time = 5;

BME280 bme280;

WiFiClient espClient;
PubSubClient client(espClient);
WiFiHandler wifiHandler(ssid, password);

void reconnect() {
  while (!client.connected()) {
    Serial.print("\u0141\u0105czenie z brokerem MQTT...");
    if (client.connect(mac.c_str())) {
      Serial.println("Po\u0142\u0105czono");
      Serial.println(mac);
      client.subscribe(("/frequency/" + mac).c_str()); // Subscribe to /frequency/{mac}

      // Send request for frequency
      String requestPayload = "{";
      requestPayload += "\"mac\":\"" + mac + "\"}";
      client.publish("/request/frequency", requestPayload.c_str());
      Serial.println("Wys\u0142ano \u017c\u0105danie /request/frequency");
    } else {
      Serial.print("B\u0142\u0105d: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

String generate_payload() {
  float temperature = bme280.readTemperature();
  float humidity = bme280.readHumidity();
  float pressure = bme280.readPressure() / 100.0F; // W hPa
  float air_quality = 50.0; // Tutaj mo\u017cesz doda\u0107 inny czujnik lub sta\u0142\u0105

  String payload = "{";
  payload += "\"temperature\":" + String(temperature, 2) + ",";
  payload += "\"humidity\":" + String(humidity, 2) + ",";
  payload += "\"pressure\":" + String(pressure, 2) + ",";
  payload += "\"air_quality\":" + String(air_quality, 2) + ",";
  payload += "\"mac\":\"" + String(mac);
  payload += "\"}";

  return payload;
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  String topicStr = String(topic);
  String expectedTopic = "/frequency/" + mac;

  if (topicStr == expectedTopic) {
    String messageStr;
    for (unsigned int i = 0; i < length; i++) {
      messageStr += (char)message[i];
    }

    // Parse the received message to extract delay_time
    int newDelayTime = messageStr.toInt(); // Assuming payload is just the delay time as a string
    if (newDelayTime > 0) {
      delay_time = newDelayTime;
      Serial.print("Zaktualizowano delay_time: ");
      Serial.println(delay_time);
    } else {
      Serial.println("Nieprawid\u0142owa warto\u015b\u0107 delay_time");
    }
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(33, 32);
  wifiHandler.connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback); // Set the MQTT callback function

  if (!bme280.begin(0x76, &Wire)) {
    Serial.println("Nie znaleziono czujnika BME280!");
    while (1);
  }

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
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  String payload = generate_payload();
  String topic = String("/data");
  client.publish(topic.c_str(), payload.c_str());

  delay(delay_time*1000);
}
