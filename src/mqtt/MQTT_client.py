import paho.mqtt.client as mqtt
import json

BROKER_ADDRESS = "127.0.0.1" 
BROKER_PORT = 1885 
MQTT_USER = "user2"   
MQTT_PASSWORD = "password"  
USER_ID = "2"
DEVICE_ID = "device_1"                    

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Połączono z brokerem MQTT")
        topic = f"/users/{USER_ID}/devices/{DEVICE_ID}/data"
        client.subscribe(topic)
        print(f"Subskrybujesz temat: {topic}")
    else:
        print(f"Błąd połączenia z brokerem MQTT. Kod: {rc}")

def on_message(client, userdata, msg):
    print(f"Odebrano wiadomość z tematu {msg.topic}: {msg.payload.decode()}")
    try:
        payload = json.loads(msg.payload.decode())
        print(f"Temperatura: {payload['temperature']}")
        print(f"Pressure: {payload['pressure']}")
        print(f"Humidity: {payload['humidity']}")
        print(f"Air quality: {payload['air_quality']}")
    except json.JSONDecodeError:
        print("Niepoprawny format JSON")

client = mqtt.Client("PC_Client")
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)

client.on_connect = on_connect
client.on_message = on_message

print("Łączenie z brokerem MQTT...")
client.connect(BROKER_ADDRESS, BROKER_PORT)

print("Oczekiwanie na wiadomości...")
client.loop_forever()
