#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int Trimr = A0;

int Trimr_Hodnota;
int Hodnota;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";


void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  Trimr_Hodnota = analogRead(Trimr);
  Trimr_Hodnota = map(Trimr_Hodnota ,0 ,1023, 0, 255); 
  Serial.print("Hodnota(0-255): ");
  Serial.println(Trimr_Hodnota);
  Hodnota = Trimr_Hodnota;
  delay(200);
  radio.write(&Hodnota, sizeof(Hodnota));
}
