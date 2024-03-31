/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

#include <DHT.h>

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

#define pinDHT 2
#define typDHT11 DHT11

DHT mojeDHT(pinDHT, typDHT11);

float tep;
float vlh;
 
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long previousMillis = 0;
const long interval = 1000;

String sTep, sVlh;

String readTemp() { 
  return String(sTep);
  //return String(teplota);
}

String readHumi() {
  return String(sVlh);
  //return String(123456789);
}

void sendSensor()
{
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
  } else {

    sTep = tep;
    sVlh = vlh;
    
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" Stupnu Celsia, ");
    Serial.print("Vlhkost: "); 
    Serial.print(vlh);
    Serial.println("%");
  }
}

void setup(){
  // Serial port for debugging purposes
  mojeDHT.begin();
  
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("\nAP IP address: ");
  Serial.println(IP);
  
  bool status;

  // Start server
  server.begin();
  
}
 
void loop(){
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;
    sendSensor();
   
    server.on("/teplota", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
      });
    server.on("/vlhkost", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
      });

  }

  
}
