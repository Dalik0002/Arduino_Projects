#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definice OLED displeje
#define SCREEN_WIDTH 128 // OLED šířka displeje v pixelech
#define SCREEN_HEIGHT 64 // OLED výška displeje v pixelech

// Deklarace objektu displeje
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// enkoder
int pinCLK = 3;
int pinDT  = 4;
int pinSW  = 5;

int poziceEnkod = 0;
int stavPred;
int stavCLK;
int stavSW;

// tlacitko
const int buttonPin = 2;

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int sett_flag = 0;

int momPozice = 0;
unsigned int MYtime = 30;

void setup(void) {

  // Inicializace displeje
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adresa 0x3C pro 128x64
    Serial.println(F("Nelze inicializovat SSD1306"));
    for (;;); // Nechte smyčku běžet navždy, pokud selže inicializace
  }
  // Vymaže obsah displeje
  display.clearDisplay();

  // Nastavení velikosti textu
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);

  // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  // nastavení propojovacího pinu pro tlačítko jako vstupní s pull up odporem
  pinMode(pinSW, INPUT_PULLUP);

  pinMode(buttonPin, INPUT);

  // pro otočení displeje o 180 stupňů
  // stačí odkomentovat řádek níže
  // mujOled.setRot180();
}

void loop(void) {

  // čtení tlačítka
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        /*Serial.println("Jsi vy nastaveni");
        poziceEnkod = hodnotaEnkoderu(poziceEnkod);*/
        if (sett_flag == 1 ){
          sett_flag = 0;
        }else if(sett_flag == 0){
          sett_flag = 1;
        }
      }
    }
  }
  lastButtonState = reading;

  delay(10);

  if (sett_flag == 1){
  display.clearDisplay();
  // funkce pro nastavení
  //režim nastavení
  hodnotaEnkoderu();
 }

 if (sett_flag == 0){
  display.clearDisplay();
  // funkce pro nastavení 
  vykresli();
 }

}

int hodnotaEnkoderu() {

  stavCLK = digitalRead(pinCLK);

  if (stavCLK != stavPred) {
    if (digitalRead(pinDT) != stavCLK) {
      Serial.print("Rotace vpravo => | ");
      poziceEnkod++;
    } else {
      Serial.print("Rotace vlevo  <= | ");
      poziceEnkod--;
    }
    Serial.print("Pozice enkoderu: ");
    Serial.println(poziceEnkod);
  }
  stavPred = stavCLK;
  momPozice = poziceEnkod;
  
   // Umístění textu
  display.setCursor(0, 10);

  // Vypsání textu
  display.print(F(poziceEnkod));

  // Aktualizace displeje s novým obsahem
  display.display();

  delay(20);

  /*stavCLK = digitalRead(pinCLK);

    if (stavCLK != stavPred) {
      if (digitalRead(pinDT) != stavCLK) {
        Serial.print("Rotace vpravo => | ");
        poziceEnkod++;
      } else {
        Serial.print("Rotace vlevo  <= | ");
        poziceEnkod--;
      }
      Serial.print("Pozice enkoderu: ");
      Serial.println(poziceEnkod);
    }
    stavPred = stavCLK;

    mujOled.setFont(u8g_font_unifont);
    mujOled.setPrintPos(0, 10);
    mujOled.print(poziceEnkod);

    momPozice = poziceEnkod;*/

}

// funkce vykresli pro výpis informací na OLED
void vykresli(void) {
  
  if(momPozice > 0){

    int time = millis();
    if (time >= MYtime){
      //mujOled.setPrintPos(20, 50);
      //mujOled.print(momPozice);
      MYtime += 1000;
      momPozice--;
    }
  
    display.setCursor(0, 10);
    display.print(F("odpocet"));

    display.setCursor(20, 50);
    display.print(F(momPozice));

    display.display();
    delay(20);

  }else{

    display.setCursor(0, 10);
    display.print(F("Mas nulovou nebo zapornou hodnotu bracho"));

    display.setCursor(20, 50);
    display.print(F(momPozice));

    display.display();
    delay(20);

  }
}

/*// funkce vykresli pro výpis nastavení enkoderu na OLED
void vykresliVnastaveni(void) {
  mujOled.setFont(u8g_font_unifont);
  mujOled.setPrintPos(0, 10);
  mujOled.print(poziceEnkod);
// mujOled.nextPage();
     
  /*for (int i = 30; i >= 0; i--) {
    mujOled.setPrintPos(0, 25);
    mujOled.print(i);
   // Přidejte zpoždění 1 sekundy pro odpočítávání
  }
}*/
