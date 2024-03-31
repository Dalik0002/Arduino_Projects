// OLED displej přes I2C
// řadič SSD1306

// připojení knihovny U8glib
#include "U8glib.h"


// inicializace OLED displeje z knihovny U8glib
U8GLIB_SSD1306_128X64 mujOled(U8G_I2C_OPT_NONE);

// proměnná pro uchování času poslední obnovy displeje
long int prepis = 0;

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

  mujOled.setFont(u8g_font_unifont);
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

  int reading = digitalRead(buttonPin);

  if (sett_flag == 1){
  // funkce pro nastavení
  //režim nastavení
  hodnotaEnkoderu();
 }

 if (sett_flag == 0){
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
  
  mujOled.firstPage();  
  do
  {
    mujOled.setPrintPos(0, 10);
    mujOled.print(poziceEnkod);

  } while( mujOled.nextPage() );
  delay(50);

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
  
    mujOled.firstPage();  
    do{
      mujOled.setPrintPos(0, 10);
      mujOled.print("odpocet");
  
      mujOled.setPrintPos(20, 50);
      mujOled.print(momPozice);
    } while( mujOled.nextPage() );
    delay(20);

  }else{

    mujOled.firstPage();  
    do{

      mujOled.print("Mas nulovou nebo zapornou hodnotu bracho");

    } while( mujOled.nextPage() );
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
