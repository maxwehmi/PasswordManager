#include <Wire.h>
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

ExternalEEPROM myMem;
const int buttonPin = 4;
int currentNextAddr = 0;// Next free address

void setup() {
  // Set up
  Wire.begin();
  myMem.setMemoryType(16);
  Keyboard.begin(KeyboardLayout_de_DE);
  pinMode(buttonPin,INPUT);

  delay(5000); // Wait so Serial can start properly

  awaitShortClick(); // Waits for the user to click once, to start the password manager

  if (myMem.begin() == false) { // If no memory is detected, something went wrong
    printString("UH OH");
    while (true);
  }

  printString("BOOTED!\n");

  showData();
}

// Cycles through the data in memory, one String at a time
void showData() {
  if (myMem.read(0) == 0) { // If the first offset is loopback, there is no data
    printString("No Data avalable.\n");
  } else {
    String currentString = "";
    int l = 0;
    while (true) { // Cycle through the data continously
      myMem.getString(currentNextAddr + 1, currentString); // Get the next String
      l = printString(currentString); // And print it
      awaitShortClick(); // When the user wants to continue and see the next String,
      deleteChars(l); // Delete the current String
      currentNextAddr += myMem.read(currentNextAddr); // Get the next address (by adding the offset to the current one)
      if (myMem.read(currentNextAddr) == 0) { // If the new offset is loopback, 
        currentNextAddr = 0; // go back to the start
      }
    }
  }
}

// Waits for the button to be pressed and return true, if the click was short (<=300ms) and false if it was long (>300ms)
bool awaitShortClick() {
  int buttonOnTime = 0;
  bool buttonOn = false; // Could be removed and the condition in 5 lines replaced by buttonOnTime>0
  while (true) { // Tests every 10ms if the button is pressed
    if (digitalRead(buttonPin) == HIGH) { // If the button is pressed,
      buttonOn = true; // it is saved that it was on and
      buttonOnTime++; // increases the timer
    } else if (buttonOn) { // If the button is not pressed, but was pressed, the click is over
      return (buttonOnTime <= 30); // Returns true, if the button was pressed for less than 30 ticks, so less than 300ms
    }
    delay(10);
  }
}

// Prints the given String and returns its length
int printString(String s) {
  Keyboard.print(s);
  return s.length();
}

// Deletes length charaters by pressing backspace length amount of times
void deleteChars(int length) {
  for (int i = 0; i < length; i++) {
    Keyboard.write(8);
  }
}

void loop(){
}