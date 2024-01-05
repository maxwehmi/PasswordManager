//#include "OneButton.h"
#include <Keyboard.h>
#include <Keyboard_de_DE.h> // Has to be changed depending on the users Keyboard Layout or the user has to switch to a german layout temporarily
// maybe this information could also be saved on the sd card

// Constants
const int buttonPin = 2;

// Global variables
bool buttonOn = false;
int buttonOnTime = 0;

//OneButton button(2, true);

void setup() {
  Keyboard.begin(KeyboardLayout_de_DE); // see the comment after #include <Keyboard_de_DE.h>

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin,INPUT);
  //button.attachClick(shortClick);
  //button.attachLongPressStop(longClick);
}

void loop() { 
  // test if the button is pressed
  //button.tick();
  delay(50);
  if (digitalRead(buttonPin) == HIGH) {
    buttonOn = true;
    buttonOnTime++;
  } else if (buttonOn) {
    if (buttonOnTime <= 4) {
      shortClick();
    } else {
      longClick();
    }
    buttonOn = false;
    buttonOnTime = 0;
  }

}
void shortClick() {
  Keyboard.println("short");
  delay(200);
}

void longClick() {
  Keyboard.println("long");
  delay(200);
}