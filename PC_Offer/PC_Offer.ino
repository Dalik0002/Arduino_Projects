/*
  Keyboard logout

  This sketch demonstrates the Keyboard library.

  When you connect pin 2 to ground, it performs a logout.
  It uses keyboard combinations to do this, as follows:

  On Windows, CTRL-ALT-DEL followed by ALT-l
  On Ubuntu, CTRL-ALT-DEL, and ENTER
  On OSX, CMD-SHIFT-q

  To wake: Spacebar.

  Circuit:
  - Arduino Leonardo or Micro
  - wire to connect D2 to ground

  created 6 Mar 2012
  modified 27 Mar 2012
  by Tom Igoe

  This example is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardLogout
*/

#include "Keyboard.h"
#include "Mouse.h"

int Memory = 0;
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  while (Memory == 1) {
    if(digitalRead(2) == HIGH)
    Memory == 0;
    delay(500);
  }
  delay(500);

  if (digitalRead(2) == LOW) {
     delay(500);
      // CTRL-ALT-DEL:
     /* Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(100);
      Keyboard.releaseAll();*/

      for(int a = 0; a <= 20;a++ ){
        Mouse.move(2000,-2000);
        delay(10);
        }
        delay(100);
      Mouse.click(MOUSE_LEFT);
      delay(100);
      Mouse.move(0,-35);
      delay(100);
      Mouse.click(MOUSE_LEFT);
      delay(100);
      Mouse.move(5,-38);
      delay(100);
      Mouse.click(MOUSE_LEFT);
      Memory++;
      
}
}
