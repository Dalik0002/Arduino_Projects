#include <DHT.h>

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLFj1tA8HB"
#define BLYNK_DEVICE_NAME "PokusJedna"
#define BLYNK_AUTH_TOKEN "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DESKTOP-IF5CDUO";
char pass[] = "123456789";

#define DHTPIN 27                      
#define DHTTYPE DHT22 
BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE); 

float vlh;                                    
float tep;  

void sendSensor()
{ 
  WidgetLED led_ERR(V2);
  WidgetLED led_ONLINE(V1);
  
  tep = dht.readTemperature();
  vlh = dht.readHumidity(); 
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
    led_ONLINE.off();
    led_ERR.on();
    
  } else {
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" Stupnu Celsia, ");
    Serial.print("Vlhkost: "); 
    Serial.print(vlh);
    Serial.println("%");

    led_ERR.off();
    led_ONLINE.off();
    delay(100);
    led_ONLINE.on();
    delay(100);  
    led_ONLINE.off();
    delay(100);
    led_ONLINE.on();
    
    Blynk.virtualWrite(V0, tep);
    Blynk.virtualWrite(V3, vlh);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
   
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  
  timer.run();
}
