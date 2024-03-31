/*
*       Example 1 - Receiver Code
*/
#include <Arduino.h>
#include <TM1637Display.h>

// Module 1 connection pins (Digital Pins)
#define CLK1 5
#define DIO1 6

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

int i;
float tep = 0;

// You can set the individual segments per digit to spell words or create other symbols:

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

   uint8_t data_2[] = { 0x0, 0x0, 0x0, 0x0 };  // all segments clear 

const byte address[6] = "00001";

struct Data_Package{
  byte tep11;
  byte vlh11;
  byte tep22;
  byte vlh22;
  };
  Data_Package data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Robojax.com two or more TM1637 Display 20181029  
  display1.setBrightness(0x0f);// set brightness of dispaly 1 

 
  display1.setSegments(data_2);// fill display 1 with whatever data[] array has  
// Robojax.com two or more TM1637 Display 20181029 

}

void loop() {
  if (radio.available()) {
    /*char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    radio.read(&i, sizeof(i));
    Serial.println(i);*/

    radio.read(&data, sizeof(Data_Package));

    display1.setSegments(data_2);
    display1.showNumberDec(data.tep11, false, 2,0);
    display1.setSegments(celsius, 2, 2);

    Serial.print("Teplota: "); 
    Serial.print(data.tep11);
    Serial.print(", stupnu Celsia ");
    
    delay(TEST_DELAY);

    display1.setSegments(data_2);
    display1.showNumberDec(data.vlh11, false, 2,0);
    display1.setSegments(procent, 2, 2);

    Serial.print("Vlhkost: "); 
    Serial.print(data.vlh11);
    Serial.println("%");
    
    delay(TEST_DELAY);



    display1.setSegments(data_2);
    display1.showNumberDec(data.tep22, false, 2,0);
    display1.setSegments(celsius, 2, 2);

    Serial.print("Teplota: "); 
    Serial.print(data.tep22);
    Serial.print(", stupnu Celsia ");
    
    delay(TEST_DELAY);

    display1.setSegments(data_2);
    display1.showNumberDec(data.vlh22, false, 2,0);
    display1.setSegments(procent, 2, 2);

    Serial.print("Vlhkost: "); 
    Serial.print(data.vlh22);
    Serial.println("%");
    
    delay(TEST_DELAY);

  }
  else
    Serial.println("Failed");
  }
