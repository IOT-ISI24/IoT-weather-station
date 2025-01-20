#include <Arduino.h>
#include <WiFi.h>
#include "ArduinoNvs.h"

constexpr int pin_config_btn = 32;

const char* config_ssid = "ESP32-AP-Config";
const char* config_password = "zaq1@WSX";

WiFiServer server(80);
WiFiClient espclient;
bool config_mode = false;


void setup() 
{
	Serial.begin(115200);
	pinMode(pin_config_btn, INPUT_PULLUP);
	NVS.begin();

	delay(5000);

	if(digitalRead(pin_config_btn) == LOW)
		config_mode = true;
	
	if(config_mode)
	{
		WiFi.mode(WIFI_AP);

		WiFi.softAP(config_ssid, config_password);
		Serial.print("IP: ");
		IPAddress IP = WiFi.softAPIP();
		Serial.println(IP);

		server.begin();
	}
	else
	{
		String net_ssid = NVS.getString("net_ssid");
		String net_pass = NVS.getString("net_pass");

		WiFi.mode(WIFI_STA);
		WiFi.begin(net_ssid, net_pass);
	}
}

void config_loop();

void loop() 
{
	if(config_mode)
	{
		config_loop();
		return;
	}
	
	while(WiFi.status() != WL_CONNECTED)
	{
		WiFi.reconnect();
		Serial.println("Not connected. Attempting to reconnect...");
		delay(5000);
	}

	if(WiFi.status() == WL_CONNECTED)
	{
		Serial.print("RSSI signal: ");
		Serial.println(WiFi.RSSI());
	}
	
	delay(1000);
}

void config_loop()
{
	espclient = server.available();

	if(espclient)
	{
		while(espclient.connected())
		{
			if(espclient.available())
			{
				String header = espclient.readString();
				Serial.println(header);

				espclient.println("HTTP/1.1 200 OK");
				espclient.println("Content-type:text/html");
				espclient.println("Connection: close");
				espclient.println();

				espclient.println("<!DOCTYPE html><html><body><form method=\"POST\"><input type=\"text\" name=\"ssid\"><input type=\"text\" name=\"pass\"><input type=\"submit\"></form></body></html>");
				espclient.println();

				if(header.startsWith("POST"))
				{
					int req_body = header.lastIndexOf("\n");
					if(req_body == -1)
					{
						Serial.println("???");
						break;
					}

					int ssid_idx = 0, passw_idx = 0, sep_idx = 0;
					if(	(ssid_idx = header.indexOf("ssid=", req_body)) != -1 	&&
						(sep_idx = header.indexOf('&', req_body)) != -1 		&&
						(passw_idx = header.indexOf("pass=", req_body)) != -1)
					{
						String req_ssid, req_pass;
						if(ssid_idx < sep_idx && sep_idx < passw_idx)
						{
							req_ssid = header.substring(ssid_idx + 5, sep_idx);
							req_pass = header.substring(sep_idx + 6);
						}
						else if(passw_idx < sep_idx && sep_idx < ssid_idx)
						{
							req_ssid = header.substring(sep_idx + 6);
							req_pass = header.substring(passw_idx + 5, sep_idx);
						}

						Serial.print("Found ssid: ");
						Serial.println(req_ssid);
						Serial.println(req_ssid.length());

						if(req_ssid.length() == 0)
							break;

						NVS.setString("net_ssid", req_ssid, false);

						Serial.print("Found passw: ");
						Serial.println(req_pass);
						Serial.println(req_pass.length());

						NVS.setString("net_pass", req_pass, false);
						NVS.commit();
						config_mode = false;
					}
				}

				break;
			}
		}

		espclient.stop();
		Serial.println("Disconnected client");
	}
	
        if(!config_mode)
	{
		WiFi.mode(WIFI_STA);
		String net_ssid = NVS.getString("net_ssid");
		String net_pass = NVS.getString("net_pass");
		WiFi.begin(net_ssid, net_pass);
	}
}
