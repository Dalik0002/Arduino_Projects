// Ultrazvukový modul HY-SRF05 pro měření vzdálenosti

// připojení potřebné knihovny
#include <NewPing.h>
// nastavení propojovacích pinů
#define pinTrigger    3
#define pinEcho       4
#define maxVzdalenost 450
// inicializace měřícího modulu z knihovny
NewPing sonar(pinTrigger, pinEcho, maxVzdalenost);

void setup() {
  // zahájení komunikace po sériové lince
  Serial.begin(9600);
}

void loop() {
  // načtení vzdálenosti v centimetrech do vytvořené proměnné vzdalenost
  int vzdalenost = sonar.ping_cm();
  // pauza před dalším měřením
  delay(50);
  // pokud byla detekována vzdálenost větší než 0,
  // provedeme další měření
  if (vzdalenost > 0) {
    vzdalenost = 0;
    // pro získání stabilnějších výsledků provedeme 5 měření
    // a výsledky budeme přičítat do proměnné vzdalenost
    for (int i = 0; i < 5; i++) {
      vzdalenost += sonar.ping_cm();
      delay(50);
    }
    // v proměnné vzdálenost máme součet posledních 5 měření
    // a musíme tedy provést dělení 5 pro získání průměru
    vzdalenost = vzdalenost / 5;
    // vytištění informací po sériové lince
    Serial.print("Vzdalenost mezi senzorem a predmetem je ");
    Serial.print(vzdalenost);
    Serial.println(" cm.");
  }
  // pokud byla detekována vzdálenost 0, je předmět mimo měřící rozsah,
  // tedy příliš blízko nebo naopak daleko
  else {
    Serial.println("Vzdalenost mezi senzorem a predmetem je mimo merici rozsah.");
    delay(500);
  }
}
