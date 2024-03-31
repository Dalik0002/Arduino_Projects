
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN

Servo servo1;
Servo servo2;


int servoVal;
int LED = 2;
int tlacitko;

const byte address[6] = "00001";

struct Data_Package{
  int servoVal1;
  int servoVal2;
  int TL;
  };
  Data_Package data;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  servo1.attach(3);
  servo2.attach(4);
}

void loop() {
  if (radio.available()) {
    
    radio.read(&data, sizeof(Data_Package));

    Serial.print("Osa X:");
    Serial.println(data.servoVal1);
    Serial.print("Osa Y:");
    Serial.println(data.servoVal2);
    Serial.print("Tlačítko: ");
    Serial.println(data.TL);

    tlacitko = data.TL;
    if(tlacitko == 0){
    digitalWrite(LED, HIGH);
        }else 
          digitalWrite(LED, LOW);
    
    servoVal = data.servoVal1;
    servo1.write(servoVal);
    
    servoVal = data.servoVal2;
    servo2.write(servoVal);
    }
  }
