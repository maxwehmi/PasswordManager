#include <Keyboard.h>
#include <Keyboard_de_DE.h> // Has to be changed depending on the users Keyboard Layout or the user has to switch to a german layout temporarily
// maybe this information could also be saved on the sd card

// Constants
const int buttonPin = 2;
const String CodeString = "Code: ";

// Global variables
bool buttonOn;
int buttonOnTime;
String code;
String websites[] = {"google", "google2", "amazon", "twitter"};
String usernames[] = {"acc1", "acc2", "acc3", "acc1"};
String passwords[] = {"pw1", "pw2", "pw3", "pw4"};


void setup() {
  // Initialize
  Keyboard.begin(KeyboardLayout_de_DE); // see the comment after #include <Keyboard_de_DE.h>
  pinMode(buttonPin,INPUT); // initialize the pushbutton pin as an input:

  // The actual program
  awaitShortClick(); // Waits for the user to click once, to start the password manager
  getCode(); // Asks the user to enter the password and saves it to "code"
  insertUserdata(); // Inserts the credentials
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
  int size = sizeof(websites) / sizeof(websites[0]); // The size of the arrays is saved (maybe this can be removed if the data is read from an sd card)
  int index = 0; // The index of the current credential
  int l = printString(websites[index]);
  while (true) { // Until the user selects a set of credentials, this loop runs
    if (awaitShortClick()) { // If the button was pressed shortly, the user wants to go to the next Credential
      index = (index + 1) % size; // The index is increased by one (modulo the size, to loop back around at the end)
      deleteChars(l); // It deletes the name of the last set of credentials and
      l = printString(websites[index]); // prints the next one
    } else { // If the button was pressed long, the user wants to use this set of credentials
      deleteChars(l); // The name is deleted to properly insert the username
      printString(usernames[index]); // The username is inserted
      awaitShortClick(); // The user is expected to tap the button to notify the password manager that they have nacvigated to the password field
      printString(decrypt(passwords[index])); // Inserts the password
      break; // Breaks the loop to end the service
    }
  }
}

// Waits for the button to be pressed and return true, if the click was short (<=300ms) and false if it was long (>300ms)
bool awaitShortClick() {
  buttonOnTime = 0;
  buttonOn = false;
  while (true) { // Tests every 10ms if the button is pressed
    if (digitalRead(buttonPin) == HIGH) { // If the button is pressed,
      buttonOn = true; // it is saved that it was on and
      buttonOnTime++; // increases the timer
    } else if (buttonOn) { // If the button is not pressed, but was pressed, the click is over
      if (buttonOnTime <= 30) { // If the click time was less than 30 ticks, so less than 300ms,
        return true; // the method returns true
      } else { //Otherwise it was a long click and
        return false; //the method returns false
      }
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