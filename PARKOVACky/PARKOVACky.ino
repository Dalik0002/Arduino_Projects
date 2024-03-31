#include <HCSR04.h>

HCSR04 hc(3,4);//initialisation class HCSR04 (trig pin , echo pin)


const int buzzer = 7; // pin buzzer

void setup(){
 pinMode(buzzer,OUTPUT);

}

void loop(){
  int vzdalenost = hc.dist();
if (vzdalenost < 100) {
  tone(buzzer,1000);
     delay(40);
   noTone(buzzer);
     delay(vzdalenost*2);
   }
 delay(100); 
} 
