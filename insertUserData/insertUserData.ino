#include <Wire.h>
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

// Constants
ExternalEEPROM myMem;
const int buttonPin = 4;
int currentNextAddr = 0;// Next free address
const String CodeString = "Code: ";
bool savingMode = false;

// Global variables
String code;
String currentData[3];

void setup() {
  // Set up
  Wire.begin();
  myMem.setMemoryType(16);
  pinMode(buttonPin,INPUT);
  Serial.begin(9600);

  delay(5000); // Wait so Serial can start properly

  if (myMem.begin() == false) { // If no memory is detected, something went wrong
    printString("UH OH");
    while (true);
  }
  Serial.println("BOOTED!");

  if(awaitShortClick()) { // A short click means insertion mode
    Serial.println("Entering Insertion mode");
    Keyboard.begin(KeyboardLayout_de_DE);
    getCode(); // Asks the user to enter the password and saves it to "code"
    insertUserdata(); // Inserts the credentials
    Keyboard.end();
  } else { // A long click means saving data mode
    Serial.println("Entering Saving mode");
    currentNextAddr = myMem.read(0);
    while (myMem.read(currentNextAddr) != 0) {
      currentNextAddr += myMem.read(currentNextAddr);
    }
    savingMode = true;
  }
}

// Asks the user to enter a 4-digit code to decrypt the passwords later on. The code is saved in the global variable "code"
void getCode() {
  printString(CodeString);
  code = "";
  for (int i = 0; i < 4; i++) { // Each digit is entered individually
    int currentDigit = 0; 
    printString(String(currentDigit));
    while (true) { // Until the user confirms the current digit with a long press, this loop runs
      if (awaitShortClick()) { // The button was pressed shortly, the user wants to increase the current digit by 1
        currentDigit = (currentDigit + 1) % 10; // The digit is increased (modulo 10, so it stays one digit)
        deleteChars(1); // The old current digit is deleted and
        printString(String(currentDigit)); // The new one is printed
      } else { // If button was pressed long, the user wants to confirm the current digit
        code += String(currentDigit); // The current digit is saved to the code and
        break; // the loop breaks to enter the next digit (or finish entering the code if it was the las one)
      }
    }
  }
  deleteChars(CodeString.length() + code.length()); // The current display is deleted, such that the user name can be entered properly
}

// Cycles through the data in memory, one String at a time
void insertUserdata() {
  if (myMem.read(0) == 0) { // If the first offset is loopback, there is no data
    printString("No Data avalable.\n");
  } else {
    String currentString = "";
    int l = 0;
    while (true) { // Cycle through the data continously
      myMem.getString(currentNextAddr + 1, currentString); // Get the next String
      splitString(currentString, char(9)); // Split it int name, username, password
      l = printString(currentData[0]); // And print the name
      if (awaitShortClick()) {// When the user wants to continue and see the next String,
        deleteChars(l); // Delete the current String
        currentNextAddr += myMem.read(currentNextAddr); // Get the next address (by adding the offset to the current one)
        if (myMem.read(currentNextAddr) == 0) { // If the new offset is loopback, 
          currentNextAddr = 0; // go back to the start
        }
      } else { // If the user wants this set of creddentials, 
        deleteChars(l); // delete the name and enter
        printString(currentData[1]); // username and
        awaitShortClick();
        printString(decrypt(currentData[2])); // password        
        break;
      }
    }
  }
}

// Splits a given String at split into three substrings
void splitString(String str, char split) {
  int StringCount = 0;
  while (str.length() > 0) { 
    int index = str.indexOf(split);
    if (index == -1) { // The String does not contain the splitting character (anymore)
      currentData[StringCount++] = str;
      break;
    } else {
      currentData[StringCount++] = str.substring(0, index); 
      str = str.substring(index+1);
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

// Writes s into the next free address
void write(String s) {
  int nextAddr = myMem.putString(currentNextAddr + 1, s); // The String has to be written one byte later than the next free address,
                                                          // because that address is reserved for the offset
  myMem.write(currentNextAddr,byte(nextAddr-currentNextAddr)); // Save the offset
  myMem.write(nextAddr,0); // Set the next offset to loopback
  currentNextAddr = nextAddr; // The new next free address is the one after the inserted String
  Serial.println("NextAddr: " + String(currentNextAddr));
}

// Erases all memory
void wipe() {
  myMem.erase();
  currentNextAddr = 0; // There is no data, so the first address is the loopback
  Serial.println("Wiped!");
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

// Decrypts the given string with the saved code
String decrypt(String password) {
  // TODO
  return password;
}

void loop(){
  // Data is currently saved in memory via Serial
  if ((Serial.available() > 0) && savingMode) { // If there is data in Serial and it is in saving mode
    String s = Serial.readStringUntil('\n'); // Read it until the end of the line
    if (s == "wipe"){ // If its the wipe command,
      wipe(); // wipe the memory
    } else { 
      Serial.println("I received: " + s);
      write(s); // Otherwise save the received data
    }
  }
}