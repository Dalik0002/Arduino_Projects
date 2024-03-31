
#include <Arduino.h>
#include <TM1637Display.h>

#include "DHT.h"

// Module 1 connection pins (Digital Pins)
#define CLK1 5
#define DIO1 6

#define pinDHT 9
#define typDHT11 DHT11
DHT mojeDHT(pinDHT, typDHT11);

// You can set the individual segments per digit to spell words or create other symbols:
const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t LOL[] = {
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_F | SEG_E | SEG_D,                           // L
};


// Create degree Celsius symbol:
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

// Create degree Celsius symbol:
const uint8_t procent[] = {
  SEG_A | SEG_G | SEG_B | SEG_F | SEG_D,  // 1/2 procenta
  SEG_D | SEG_E | SEG_G | SEG_A | SEG_C  // 2/2 procenta
};

// The amount f time (in milliseconds) between tests
#define TEST_DELAY   5000



TM1637Display display1(CLK1, DIO1);// define dispaly 1 object

   uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };  // all segments clear    
   
void setup()
{
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600); 
  // zapnutí komunikace s teploměrem DHT
  mojeDHT.begin();
  
  
  // Robojax.com two or more TM1637 Display 20181029  
  display1.setBrightness(0x0f);// set brightness of dispaly 1 

 
  display1.setSegments(data);// fill display 1 with whatever data[] array has  
// Robojax.com two or more TM1637 Display 20181029  
}

void loop()
{
  // pomocí funkcí readTemperature a readHumidity načteme
  // do proměnných tep a vlh informace o teplotě a vlhkosti,
  // čtení trvá cca 250 ms
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
  } else {
    
    /*display1.setSegments(data);
    for(int i=0; i<=15; i++)
    {
      display1.showNumberDec(i);
      delay(1000);
   }
      delay(TEST_DELAY);*/

    display1.setSegments(data);
    display1.showNumberDec(tep, false, 2,0);
    display1.setSegments(celsius, 2, 2);
    
    delay(TEST_DELAY);

    display1.setSegments(data);
    display1.showNumberDec(vlh, false, 2,0);
    display1.setSegments(procent, 2, 2);
    delay(TEST_DELAY);
  
    display1.setSegments(data);
    display1.setSegments(LOL,3,1);
    delay(TEST_DELAY);
  }
  
}
 
