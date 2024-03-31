#include <OneWire.h>
#include <DallasTemperature.h>

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
char ssid[] = "Tp-ling";
char pass[] = "hromasova";

#define pin 5

// nastavení komunikace senzoru přes pin
OneWire oneWire(pin);     
// převedeme onewire do Dallasu                     
DallasTemperature sensors(&oneWire); 

BlynkTimer timer;

void sendSensor()
{ 
  WidgetLED led1(V0);
  WidgetLED led2(V1);
  
  sensors.requestTemperatures();               
  //Serial.print("Teplota je: ");
  // tento řádek je vysvětlený v článku
  //Serial.print(sensors.getTempCByIndex(0));    
  //Serial.println(" stupnu Celsia");
  delay(1000);
  
  float tep = sensors.getTempCByIndex(0);
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (tep == -127) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z čidla!"); 
    led1.off();
    led2.on();
    
  } else {
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" Stupnu Celsia, ");

    led2.off();
    led1.off();
    delay(100);
    led1.on();
    delay(100);  
    led1.off();
    delay(100);
    led1.on();
    
    Blynk.virtualWrite(V2, tep);
    //Blynk.virtualWrite(V3, vlh);
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  sensors.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();

  timer.run();
}
