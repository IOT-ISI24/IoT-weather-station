import paho.mqtt.client as mqtt
import json

# Konfiguracja połączenia MQTT
BROKER_ADDRESS = "localhost"  # Adres brokera MQTT
BROKER_PORT = 1885           # Port brokera MQTT
MQTT_USER = "default"        # Użytkownik MQTT
MQTT_PASSWORD = "default"    # Hasło MQTT

USER_ID = "123e4567-e89b-12d3-a456-426614174000"  # Identyfikator użytkownika
DEVICE_ID = "device_1"                            # Identyfikator urządzenia

# Funkcja obsługująca połączenie z brokerem MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Połączono z brokerem MQTT")
        # Subskrybuj temat z temperaturą
        topic = f"/users/{USER_ID}/devices/{DEVICE_ID}/temperature"
        client.subscribe(topic)
        print(f"Subskrybujesz temat: {topic}")
    else:
        print(f"Błąd połączenia z brokerem MQTT. Kod: {rc}")

# Funkcja obsługująca odebrane wiadomości
def on_message(client, userdata, msg):
    print(f"Odebrano wiadomość z tematu {msg.topic}: {msg.payload.decode()}")
    try:
        # Parsuj JSON
        payload = json.loads(msg.payload.decode())
        print(f"Temperatura: {payload['temperature']} °C")
    except json.JSONDecodeError:
        print("Niepoprawny format JSON")

# Konfiguracja klienta MQTT
client = mqtt.Client("PC_Client")
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)

# Rejestracja funkcji obsługi
client.on_connect = on_connect
client.on_message = on_message

# Połącz się z brokerem MQTT
print("Łączenie z brokerem MQTT...")
client.connect(BROKER_ADDRESS, BROKER_PORT)

# Rozpocznij nasłuchiwanie
print("Oczekiwanie na wiadomości...")
client.loop_forever()
