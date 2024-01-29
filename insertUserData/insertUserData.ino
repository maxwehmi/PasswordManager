#include <Wire.h>
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

ExternalEEPROM myMem;
const int buttonPin = 4;
byte DataSize = 50;
byte numOfEntries = 0;

void setup() {
  Wire.begin();
  myMem.setMemoryType(16);
  Keyboard.begin(KeyboardLayout_de_DE);
  pinMode(buttonPin,INPUT);

  delay(5000);

  awaitShortClick();

  if (myMem.begin() == false)  {
    printString("UH OH");
    while (true);
  }

  printString("BOOTED!\n");
  numOfEntries = myMem.read(0);
  printString(" I got numOfEntries: " + String(numOfEntries) + "\n");

  showData();
}

void showData() {
  if (numOfEntries == 0) {
    printString("No Data avalable.\n");
  } else {
    String currentCredentials = "";
    int l = 0;
    while (true) {
      for (int i = 0; i < numOfEntries; i++) {
        myMem.getString(10 + i * DataSize, currentCredentials);
        l = printString(currentCredentials);
        awaitShortClick();
        deleteChars(l);
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