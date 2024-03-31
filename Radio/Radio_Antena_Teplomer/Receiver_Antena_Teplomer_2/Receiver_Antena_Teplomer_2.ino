/*
*       Example 1 - Receiver Code
*/
#include <Arduino.h>
#include <TM1637Display.h>

// Module 1 connection pins (Digital Pins)
#define CLK1 5
#define DIO1 6

TM1637Display display1(CLK1, DIO1);// define dispaly 1 object

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

int i;

void Teplota(int tep);
void Vlhkost(int vlh);

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

uint8_t data_2[] = { 0x0, 0x0, 0x0, 0x0 };  // all segments clear

#define TEST_DELAY   5000

const byte address[6] = "00001";

struct Data_Package{
  byte tep;
  byte vlh;
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
    radio.read(&data, sizeof(Data_Package));
    int tep = data.tep;
    int vlh = data.vlh; 
    
    Teplota(tep);
    delay(1000);
    
    Vlhkost(vlh);
    delay(1000);
    
      }
  }

void Teplota(int tep){
    

    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.println(" stupnu Celsia ");

    display1.setSegments(data_2);
    display1.showNumberDec(tep, false, 2,0);
    display1.setSegments(celsius, 2, 2);

   //data.tep = 0;
   //data.vlh = 0;
  }

void Vlhkost(int vlh){
    //radio.read(&data, sizeof(Data_Package));

    Serial.print("Vlhkost: "); 
    Serial.print(vlh);
    Serial.println("%");

    display1.setSegments(data_2);
    display1.showNumberDec(vlh, false, 2,0);
    display1.setSegments(procent, 2, 2);

    //data.tep = 0;
    //data.vlh = 0;
  }
