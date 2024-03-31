#include <Arduino.h>
#include <TM1637Display.h>
int tlac = 12;

#define CLK1 5
#define DIO1 6

const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t LOL[] = {
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_F | SEG_E | SEG_D,                           // L
};

 uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };  // all segments clear 

  TM1637Display display1(CLK1, DIO1);// define dispaly 1 object
 
void setup() {
  // put your setup code here, to run once:
pinMode(12, INPUT);

 // Robojax.com two or more TM1637 Display 20181029  
  display1.setBrightness(0x0f);// set brightness of dispaly 1 

 
  display1.setSegments(data);// fill display 1 with whatever data[] array has  
// Robojax.com two or more TM1637 Display 20181029 

}

void loop() {
  // put your main code here, to run repeatedly:
   int statusSensor = digitalRead (tlac);
  if(statusSensor == 0){
  display1.setSegments(data);
    for(int i=0; i<=60--; i++)
    {
      display1.showNumberDec(i);
      delay(1000);
   }
      delay(500);
  }else{
    display1.setSegments(data);
    display1.setSegments(LOL,3,1);
    }
   
}
