#include "DHT.h"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "bVf2KmFjlC6NJdt2aF0S-sucU6pcnRQV";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Bydlenky";
char pass[] = "nebezpecneznamosti";

#define pinDHT 2
#define typDHT11 DHT11
BlynkTimer timer;

DHT mojeDHT(pinDHT, typDHT11);

void sendSensor()
{ 
  WidgetLED led1(V0);
  WidgetLED led2(V1);
  
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
    led1.off();
    led2.on();
    
  } else {
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" Stupnu Celsia, ");
    Serial.print("Vlhkost: "); 
    Serial.print(vlh);
    Serial.println("%");

    led2.off();
    led1.off();
    delay(100);
    led1.on();
    delay(100);  
    led1.off();
    delay(100);
    led1.on();
    
    Blynk.virtualWrite(V2, tep);
    Blynk.virtualWrite(V3, vlh);
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  mojeDHT.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();

  timer.run();
}
