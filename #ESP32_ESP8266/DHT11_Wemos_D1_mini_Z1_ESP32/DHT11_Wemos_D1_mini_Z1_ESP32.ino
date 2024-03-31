#include "DHT.h"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLFj1tA8HB"
#define BLYNK_DEVICE_NAME "PokusJedna"
#define BLYNK_AUTH_TOKEN "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V"


//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "DESKTOP-IF5CDUO";
const char* pass =  "123456789";

#define pinDHT 2
#define typDHT11 DHT11
BlynkTimer timer;

DHT mojeDHT(pinDHT, typDHT11);

void sendSensor();

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  mojeDHT.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();

  timer.run();
}

void sendSensor()
{ 
  WidgetLED led1(V1);
  WidgetLED led2(V2);
  
  float tep = mojeDHT.readTemperature()-4;
  float vlh = mojeDHT.readHumidity()+10;
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
    led1.off();
    led2.on();
    delay(200); 
    led2.off();  
    delay(200); 
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
    
    Blynk.virtualWrite(V0, tep);
    Blynk.virtualWrite(V3, vlh);
  }
}
