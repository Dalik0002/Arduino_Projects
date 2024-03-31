
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int joyX = A5;
int joyY = A4;
int SW_pin = 2;

int servoVal;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

struct Data_Package{
  int servoVal1;
  int servoVal2;
  int TL;
  };
  Data_Package data;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  /*Serial.print("Tlačítko....: ");
  Serial.println(digitalRead(SW_pin));
  data.TL = digitalRead(SW_pin);*/
    
  servoVal = analogRead(joyX);
  servoVal = map(servoVal ,0 ,1023, 0, 255); 
  Serial.print("Osa X:");
  Serial.println(servoVal);
  data.servoVal1 = servoVal;
  

 /* servoVal = analogRead(joyY);
  servoVal = map(servoVal ,0 ,1023 ,0 ,180);
  Serial.print("Osa je Y:");
  Serial.println(servoVal);
  data.servoVal2 = servoVal;*/
  
  radio.write(&data, sizeof(Data_Package));

  delay(50);
  
}
