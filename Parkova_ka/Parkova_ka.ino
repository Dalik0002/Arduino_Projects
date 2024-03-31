#include <HCSR04.h>


#include <NewPing.h>
int buzzer = 7;
#define pinTrigger    3
#define pinEcho       4
#define maxVzdalenost 450

NewPing sonar(pinTrigger, pinEcho, maxVzdalenost);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop() {
 
  int vzdalenost = sonar.ping_cm();
  delay(50);
     vzdalenost = 0;
    for (int i = 0; i < 5; i++) {
      vzdalenost += sonar.ping_cm();
      delay(50);
    }
    vzdalenost = vzdalenost / 5;
    Serial.print("Vzdalenost mezi senzorem a predmetem je ");
    Serial.print(vzdalenost);
    Serial.println(" cm.");
    
   if (vzdalenost > 50 ) {
    digitalWrite(buzzer, LOW);
    
  }else if (vzdalenost < 50 && vzdalenost > 40) {
 
    digitalWrite(buzzer, HIGH);
    delay (800);
    digitalWrite(buzzer, LOW);
    delay (800);
    
  }else if (vzdalenost < 40 && vzdalenost > 30 ) {


    digitalWrite(buzzer, HIGH);
    delay (300);
    digitalWrite(buzzer, LOW);
    delay (300);
  }

  else if (vzdalenost < 30 && vzdalenost > 20){
 
    
    digitalWrite(buzzer, HIGH);
    delay (100);
    digitalWrite(buzzer, LOW);
    delay (100);

  }
  else if (vzdalenost < 20){
 
    digitalWrite(buzzer, HIGH);

  }
}
  /*digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay (100);*/
