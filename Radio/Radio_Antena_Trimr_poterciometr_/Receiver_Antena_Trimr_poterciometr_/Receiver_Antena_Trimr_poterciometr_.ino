#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int motor = A3;
float Napeti = 0;


int Hodnota;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.begin(9600);
}

void loop() {
  if (radio.available()) {
    
    radio.read(&Hodnota, sizeof(Hodnota));
    
    Napeti = map(Hodnota, 0,255,0,470);
    
    Serial.print("Hodnota(0-255): ");
    Serial.println(Hodnota);
    Serial.print("Napeti -------------------------------->");
    Serial.print(Napeti/100);
    Serial.println("V");
    
    analogWrite(motor,Hodnota);
    delay(200);
}
}
