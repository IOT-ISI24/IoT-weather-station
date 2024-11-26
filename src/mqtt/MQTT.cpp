#include <WiFi.h>
#include <PubSubClient.h>
#include <wifi/WiFiHandler.h>


const char* ssid = "Hotspot";
const char* password = "heineken123";


const char* mqtt_server = "192.168.196.156";
const int mqtt_port = 1885;
const char* mqtt_user = "user2";
const char* mqtt_password = "password";

const char* user_id = "2";
const char* device_id = "device_1";


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

String generate_payload(){
  float temperature = 25.0 + random(-100, 100) / 100.0;
  float air_quality = 50.0 + random(-500, 500) / 100.0;
  float pressure = 1013.0 + random(-200, 200) / 100.0;
  float humidity = 60.0 + random(-300, 300) / 100.0;
  String payload = "{";
  payload += "\"temperature\":" + String(temperature) + ",";
  payload += "\"air_quality\":" + String(air_quality) + ",";
  payload += "\"pressure\":" + String(pressure) + ",";
  payload += "\"humidity\":" + String(humidity);
  payload += "}";
  return payload;
}

void setup6() {
  Serial.begin(9600);
  wifiHandler.connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
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


