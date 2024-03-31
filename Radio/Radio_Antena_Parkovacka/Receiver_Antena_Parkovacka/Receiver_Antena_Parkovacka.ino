/*
*       Example 1 - Receiver Code
*/
#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

int i;
float vzdalenost = 0;
const int buzzer = 3;

const byte address[6] = "00001";

struct Data_Package{
  byte tep;
  byte vlh;
  };
  Data_Package data;

void setup() {
  pinMode(buzzer,OUTPUT);
   
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    /*char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    radio.read(&i, sizeof(i));
    Serial.println(i);*/

    radio.read(&vzdalenost, sizeof(vzdalenost));

    Serial.print("Vzdalenost je: ");
    Serial.print(vzdalenost);
    Serial.println(" cm.");
    
   if (vzdalenost > 50 ) {
    noTone(buzzer);
    
  }else if (vzdalenost < 50 && vzdalenost > 40) {
 
    tone(buzzer,1000);
    delay (800);
    noTone(buzzer);
    delay (800);
    
  }else if (vzdalenost < 40 && vzdalenost > 30 ) {


    tone(buzzer,1000);
    delay (300);
    noTone(buzzer);
    delay (300);
  }

  else if (vzdalenost < 30 && vzdalenost > 20){
 
    
    tone(buzzer,1000);
    delay (100);
    noTone(buzzer);
    delay (100);

  }
  else if (vzdalenost < 20){
 
    tone(buzzer,1000);

      }
    }
  }
