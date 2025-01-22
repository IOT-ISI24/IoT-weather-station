// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiProv.h>
// #include "ArduinoNvs.h"

// constexpr int pin_config_btn = 32;

// //PIN i nazwa serwisu BLE
// constexpr const char* ble_pop = "123qwe456";
// constexpr const char* ble_service = "PROV_ESP32_WEATHER_STATION";

// bool config_mode = false;
// bool connected = false;

// //Funkcja do obslugi zdarzen WiFi
// void SysWiFiEvent(arduino_event_t* ev)
// {
//     switch(ev->event_id)
//     {
//         case ARDUINO_EVENT_WIFI_STA_GOT_IP:
//         {
//             Serial.print("Connected: ");
//             Serial.println(IPAddress(ev->event_info.got_ip.ip_info.ip.addr));
//             connected = true;
//             break;
//         }
//         case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
//         {
//             Serial.println("Lost conncetion. Reconnecting...");
//             connected = false;
//             WiFi.begin();
//         }

//         case ARDUINO_EVENT_PROV_START:
//         {
//             Serial.println("Provision started, setup WiFi via bluetooth app");
//             break;
//         }
//         case ARDUINO_EVENT_PROV_CRED_RECV:
//         {
//             String ssid = (const char*)ev->event_info.prov_cred_recv.ssid;
//             String pass = (const char*)ev->event_info.prov_cred_recv.password;

//             Serial.print("Got SSID: ");
//             Serial.println(ssid);
//             Serial.print("Got pass: ");
//             Serial.println(pass);

//             NVS.setString("net_ssid", ssid, false);
//             NVS.setString("net_pass", pass, false);
//             NVS.commit();

//             break;
//         }
//         case ARDUINO_EVENT_PROV_CRED_SUCCESS:
//         {
//             Serial.println("Provision sucessful");
//             break;
//         }
//         case ARDUINO_EVENT_PROV_CRED_FAIL:
//         {
//             if(ev->event_info.prov_fail_reason == wifi_prov_sta_fail_reason_t::WIFI_PROV_STA_AP_NOT_FOUND)
//             {
//                 Serial.println("Cannot find WiFi AP");
//             }
//             else
//             {
//                 Serial.println("WiFi AP credintials incorrect");
//             }
//             break;
//         }
//         case ARDUINO_EVENT_PROV_END:
//         {
//             Serial.println("Provision successful");
//             break;
//         }
//         default:
//             break;
//     }
// }

// void setup() 
// {
// 	Serial.begin(115200);
// 	pinMode(pin_config_btn, INPUT_PULLUP);
// 	NVS.begin();

// 	delay(5000);

// 	if(digitalRead(pin_config_btn) == LOW)
// 		config_mode = true;
	
//     WiFi.onEvent(SysWiFiEvent);

//     if(config_mode)
//     {
//         NVS.erase("net_ssid", false);
//         NVS.erase("net_pass", false);
//         NVS.commit();

//         WiFiProv.beginProvision(
//             WIFI_PROV_SCHEME_BLE, 
//             WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, 
//             WIFI_PROV_SECURITY_1,
//             ble_pop, ble_service);
//     }
//     else
//     {
//         String net_ssid = NVS.getString("net_ssid");
//         String net_pass = NVS.getString("net_pass");

//         WiFi.mode(WIFI_STA);
//         WiFi.begin(net_ssid, net_pass);
//     }
// }

// void loop()
// {
//     if(connected)
//     {
//         Serial.print("Signal strength: ");
//         Serial.println(WiFi.RSSI());
//         delay(5000);
//     }
//     else
//     {
//         Serial.print("Awaiting connection/provision...");
//         delay(5000);
//     }
// }