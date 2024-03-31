#include <Adafruit_Sensor.h>

/*
*     Example 1 - Transmitter Code
*/
#include <Arduino.h>
#include "DHT.h"


#define pinDHT 3
#define typDHT11 DHT11
DHT mojeDHT11(pinDHT, typDHT11);

#define pinDHT 5
#define typDHT22 DHT22
DHT mojeDHT22(pinDHT, typDHT22);

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int i = 123456789;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

struct Data_Package{
  byte tep11;
  byte vlh11;
  byte tep22;
  byte vlh22;
  };
  Data_Package data;

void setup() {
  mojeDHT11.begin();
  mojeDHT22.begin();
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  /*const char text[] = "Hello Maaaaaaaaaaan";
    radio.write(&text, sizeof(text));
  delay(1000);
  i++;
  radio.write(&i, sizeof(i));
  delay(500);*/

  data.tep11 = mojeDHT11.readTemperature();
  data.vlh11 = mojeDHT11.readHumidity();
  delay(100);

  data.tep22 = mojeDHT22.readTemperature();
  data.vlh22 = mojeDHT22.readHumidity();
  delay(100);
  
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(data.tep11) || isnan(data.vlh11)|| isnan(data.tep22) || isnan(data.vlh22)) {
    radio.write(&i, sizeof(i));
     delay(10);
     Serial.print("Teplota: "); 
    Serial.print(i);
    Serial.println(" stupnu Celsia ");
  }else {
    radio.write(&data, sizeof(Data_Package));
    delay(500);
    Serial.print("Teplota DHT11: "); 
    Serial.print(data.tep11);
    Serial.print(", stupnu Celsia ");

    Serial.print("Vlhkost DHT11: "); 
    Serial.print(data.vlh11);
    Serial.println("%");

    Serial.print("Teplota DHT22: "); 
    Serial.print(data.tep22);
    Serial.print(", stupnu Celsia ");

    Serial.print("Vlhkost DHT22: "); 
    Serial.print(data.vlh22);
    Serial.println("%");

    Serial.println(" ");
    Serial.println(" ");
    }
}
