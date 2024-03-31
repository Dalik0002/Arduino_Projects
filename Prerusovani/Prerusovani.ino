#include <Arduino.h>
#include <TM1637Display.h>
#include "DHT.h"

#define CLK1 5
#define DIO1 6

#define pinDHT 9
#define typDHT11 DHT11
DHT mojeDHT(pinDHT, typDHT11);

#define TEST_DELAY   5000

int buttonPin = 12;
boolean run;

const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

const uint8_t procent[] = {
  SEG_A | SEG_G | SEG_B | SEG_F | SEG_D,  // 1/2 procenta
  SEG_D | SEG_E | SEG_G | SEG_A | SEG_C   // 2/2 procenta
};

TM1637Display display1(CLK1, DIO1);

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
 
void setup() {
  run = true;
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  mojeDHT.begin();
  
  
  // Robojax.com two or more TM1637 Display 20181029  
  display1.setBrightness(0x0f);// set brightness of dispaly 1 

 
  display1.setSegments(data);// fill display 1 with whatever data[] array has  
// Robojax.com two or more TM1637 Display 20181029  
}
 
void loop() {
  if(run){
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
  } 
  else {

    display1.setSegments(data);
    display1.showNumberDec(tep, false, 2,0);
    display1.setSegments(celsius, 2, 2);
    delay(TEST_DELAY);

    display1.setSegments(data);
    display1.showNumberDec(vlh, false, 2,0);
    display1.setSegments(procent, 2, 2);
    delay(TEST_DELAY);
      }
  }
    
  if(digitalRead(buttonPin) == HIGH)
    run = false;

}
