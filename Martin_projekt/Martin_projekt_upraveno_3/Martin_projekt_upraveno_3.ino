
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definice OLED displeje
#define SCREEN_WIDTH 128 // OLED šířka displeje v pixelech
#define SCREEN_HEIGHT 64 // OLED výška displeje v pixelech

// Deklarace objektu displeje
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

#define semafor_ZELENA 10
#define semafor_ORANZOVA 11
#define semafor_CERVENA 12

#define prechod_ZELENA 8
#define prechod_CERVENA 9

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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int buttonPin_prechod = 7;

int buttonState_prechod;
int lastButtonState_prechod = LOW;
unsigned long lastDebounceTime_prechod = 0;
unsigned long debounceDelay_prechod = 50;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int sett_flag = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sett_flag_prechod = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int momPozice = 50;
unsigned int MYtime = 30;

int postup = 3;
int MOMpostup = postup;


#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int L3G4200D_Address = 105; //I2C address of the L3G4200D

int x;
int y;
int z;


void setup(void) {


Wire.begin();
 Serial.begin(9600);

Serial.println("starting up L3G4200D");
 setupL3G4200D(2000);
 delay(1500);
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

  pinMode(semafor_ZELENA, OUTPUT);
  pinMode(semafor_ORANZOVA, OUTPUT);
  pinMode(semafor_CERVENA, OUTPUT);

  pinMode(prechod_ZELENA, OUTPUT);
  pinMode(prechod_CERVENA, OUTPUT);


  // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  // nastavení propojovacího pinu pro tlačítko jako vstupní s pull up odporem
  pinMode(pinSW, INPUT_PULLUP);

  pinMode(buttonPin, INPUT);
  pinMode(buttonPin_prechod, INPUT);

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
        if (sett_flag == 1 ){
          
          Serial.println("flag 0");
          sett_flag = 0;
        }else if(sett_flag == 0){
          Serial.println("flag 1");
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 if (sett_flag == 0) {
  getGyroValues();
  Serial.println("inside");
  display.clearDisplay();

  int reading_prechod = digitalRead(buttonPin_prechod);

  if (reading_prechod != lastButtonState_prechod) {
    lastDebounceTime_prechod = millis();
  }

  if ((millis() - lastDebounceTime_prechod) > debounceDelay_prechod) {
    if (reading_prechod != buttonState_prechod) {
      buttonState_prechod = reading_prechod;
      if (buttonState_prechod == HIGH) {
        if(sett_flag_prechod == 1){
          Serial.println("2flag 0");
          sett_flag_prechod = 0;
        }else if(sett_flag_prechod == 0){
          Serial.println("2flag 1");
          sett_flag_prechod = 1;
        }
      }
    }
  }
  lastButtonState_prechod = reading_prechod;

  if(y > 500 || z > 500){

    unsigned long kontrolaPohyb = 0;
    unsigned long aktualni_cas = millis();

    while(aktualni_cas + 5000 > millis()){

      if(y < 400 || z < 400){
        goto exit;
      }

    }

    if (sett_flag_prechod == 1 || y > 500 || z > 500 ) {
    if (MOMpostup == 3) {
      semafor();
      MOMpostup--;
    } else if (MOMpostup == 2) {
      vykresli();
      if (momPozice == 0) {
        MOMpostup = 0;
      }
    } else if (MOMpostup == 0) {
      puvodPozice();
      MOMpostup = postup;
      sett_flag_prechod = 0;
    }

    display.clearDisplay();
    }
  }
  
  exit:

  if (sett_flag_prechod == 0) {
    digitalWrite(prechod_CERVENA, HIGH);
    digitalWrite(semafor_ZELENA, HIGH);
    display.clearDisplay();
  }
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
     if(poziceEnkod <= 0){
      display.setCursor(0, 10);
    display.print(F("Nize to uz nejde"));
    display.display();
    delay(20);
    }else if(poziceEnkod >= 0){
      poziceEnkod--;
    }
    }
    if(poziceEnkod <= 0){

    }
    Serial.print("Pozice enkoderu: ");
    Serial.println(poziceEnkod);
  }
  stavPred = stavCLK;
  momPozice =  poziceEnkod;
  
  display.setCursor(0, 10);

  // Vypsání textu
  display.print(poziceEnkod);

  // Aktualizace displeje s novým obsahem
  display.display();

  delay(20);

}

// funkce vykresli pro výpis informací na OLED
void vykresli(void) {
  
  if(momPozice > 0){

    delay(1000);
    momPozice--;
  
    display.setCursor(0, 10);
    display.print(F("odpocet"));

    display.setCursor(20, 50);
    display.print(momPozice);

    display.display();
    delay(20);

  }else{

    display.setCursor(0, 10);
    display.print(F("Mas nulovou nebo zapornou hodnotu bracho"));

    display.setCursor(20, 50);
    display.print(momPozice);

    display.display();
    delay(20);

  }
}

void semafor(void) {
        digitalWrite(semafor_ZELENA,LOW);
        delay(1000);

        digitalWrite(semafor_ORANZOVA,HIGH);
        delay(1000);
        digitalWrite(semafor_ORANZOVA,LOW);
        delay(1000);

        digitalWrite(semafor_CERVENA ,HIGH);
        delay(1000);

        digitalWrite(prechod_CERVENA, LOW);
        

   
}

void puvodPozice(void){
  digitalWrite(prechod_ZELENA, LOW);
  delay(1000);
  digitalWrite(prechod_CERVENA,HIGH);
  delay(1000);

   digitalWrite(semafor_CERVENA ,LOW);
        delay(1000);
   digitalWrite(semafor_ORANZOVA,HIGH);
        delay(1000);
        digitalWrite(semafor_ORANZOVA,LOW);
        delay(1000);
    digitalWrite(semafor_ZELENA,HIGH);
}

void getGyroValues(){

byte xMSB = readRegister(L3G4200D_Address, 0x29);
 byte xLSB = readRegister(L3G4200D_Address, 0x28);
 x = ((xMSB << 8) | xLSB);

byte yMSB = readRegister(L3G4200D_Address, 0x2B);
 byte yLSB = readRegister(L3G4200D_Address, 0x2A);
 y = ((yMSB << 8) | yLSB);

byte zMSB = readRegister(L3G4200D_Address, 0x2D);
 byte zLSB = readRegister(L3G4200D_Address, 0x2C);
 z = ((zMSB << 8) | zLSB);
}

int setupL3G4200D(int scale){
 //From Jim Lindblom of Sparkfun's code

// Enable x, y, z and turn off power down:
 writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

// If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
 writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

// Configure CTRL_REG3 to generate data ready interrupt on INT2
 // No interrupts used on INT1, if you'd like to configure INT1
 // or INT2 otherwise, consult the datasheet:
 writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

// CTRL_REG4 controls the full-scale range, among other things:

if(scale == 250){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
 }else if(scale == 500){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
 }else{
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
 }

// CTRL_REG5 controls high-pass filtering of outputs, use it
 // if you'd like:
 writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) {
 Wire.beginTransmission(deviceAddress); // start transmission to device 
 Wire.write(address); // send register address
 Wire.write(val); // send value to write
 Wire.endTransmission(); // end transmission
}

int readRegister(int deviceAddress, byte address){

int v;
 Wire.beginTransmission(deviceAddress);
 Wire.write(address); // register to read
 Wire.endTransmission();

Wire.requestFrom(deviceAddress, 1); // read a byte

while(!Wire.available()) {
 // waiting
 }

v = Wire.read();
 return v;
}
