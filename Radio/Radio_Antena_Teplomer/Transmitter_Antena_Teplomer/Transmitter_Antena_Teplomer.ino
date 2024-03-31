#include <Adafruit_Sensor.h>

/*
*     Example 1 - Transmitter Code
*/
#include <Arduino.h>
#include "DHT.h"


#define pinDHT 5 
#define typDHT22 DHT22
DHT mojeDHT(pinDHT, typDHT22);

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int i = 0;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

struct Data_Package{
  byte tep;
  byte vlh;
  };
  Data_Package data;

void setup() {
  mojeDHT.begin();
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  /*const char text[] = "Hello Maaaaaaaaaaan";
    radio.write(&text, sizeof(text));
  delay(1000);
  i++;
  radio.write(&i, sizeof(i));
  delay(500);*/

  data.tep = mojeDHT.readTemperature();
  data.vlh = mojeDHT.readHumidity();
  delay(100);
  
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(data.tep) || isnan(data.vlh)) {
    radio.write(&i, sizeof(i));
     delay(10);
     Serial.print("Teplota: "); 
    Serial.print(i);
    Serial.println(" stupnu Celsia ");
  }else {
    radio.write(&data, sizeof(Data_Package));
    delay(500);
    Serial.print("Teplota: "); 
    Serial.print(data.tep);
    Serial.println(" stupnu Celsia ");

    Serial.print("Vlhkost: "); 
    Serial.print(data.vlh);
    Serial.println("%");
    
    }
}
