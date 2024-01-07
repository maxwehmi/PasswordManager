#include <Keyboard.h>
#include <Keyboard_de_DE.h> // Has to be changed depending on the users Keyboard Layout or the user has to switch to a german layout temporarily
// maybe this information could also be saved on the sd card
#include <SD.h>

// Constants
const int buttonPin = 2;
const int SDPin = 4;
const String CodeString = "Code: ";
const String dataPath = "passwords.csv";

// Global variables
String code;
File userdata;

void setup() {
  // Initialize
  Keyboard.begin(KeyboardLayout_de_DE); // see the comment after #include <Keyboard_de_DE.h>
  pinMode(buttonPin,INPUT); // Initialize the pushbutton pin as an input

  // The actual program
  awaitShortClick(); // Waits for the user to click once, to start the password manager
  if (getUserdata()) { // Gets the userdata from the SD Card. If it returns false, there was an error
    getCode(); // Asks the user to enter the password and saves it to "code"
    insertUserdata(); // Inserts the credentials
  }
  Keyboard.end();
}

bool getUserdata() {
  if (!SD.begin(SDPin)) {
    printString("ERROR: SD-Card not found!");
    return false;
  }
  userdata = SD.open(dataPath);
  if (!userdata) {
    printString("ERROR: Password-File not found!");
    return false;
  }
  return true;
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

// Cycles through the saved user data and enters the selected credentials
void insertUserdata() {
  String entry = "";
  int l = 0;
  bool exit = false;
  while (!exit) {
    userdata = SD.open(dataPath);
    while (userdata.available() && (!exit)) {
      entry = readNextEntry();
      l = printString(entry); // WebsiteName
      if (awaitShortClick()) {
        deleteChars(l);
        jumpToEndOfLine();
      } else {
        deleteChars(l);
        entry = readNextEntry();
        printString(entry); // username
        entry = readNextEntry();
        awaitShortClick();
        printString(entry); // password
        exit = true;
      }
    }
    userdata.close();
  }
}

String readNextEntry() {
  String entry = "";
  while (userdata.available()) {
    char m = userdata.read();
    if (((int) m == 9) || (m == '\n')){
      break;
    } else if((int) m != 13) {
      entry += m;
    }
  }
  return entry;
}

void jumpToEndOfLine() {
  while (userdata.available()) {
    char m = userdata.read();
    if (m == '\n'){
      break;
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

// Decrypts the given string with the saved code
String decrypt(String password) {
  // TODO
  return password;
}

void loop() { 
  // nothing happens after setup
}