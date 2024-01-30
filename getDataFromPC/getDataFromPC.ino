#include <Wire.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

ExternalEEPROM myMem;

int currentNextAddr = 0; // Next free address

void setup() {
  // Set up
  Wire.begin();
  myMem.setMemoryType(16);
  Serial.begin(9600);

  delay(5000); // Wait so Serial can start properly

  if (myMem.begin() == false) { // If no memory is detected, something went wrong
    Serial.print("UH OH");
    while (true);
  }

  // Each datapoint consists of a byte with the offset to the next datapoint and the String.
  // The next free Address is all the offsets summed up. The offset "0" indicates that no 
  // String follows.
  currentNextAddr = myMem.read(0);
  while (myMem.read(currentNextAddr) != 0) {
    currentNextAddr += myMem.read(currentNextAddr);
  }

  Serial.println("BOOTED!");
  Serial.println("Current Next Address: " + String(currentNextAddr));
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

void loop() {
  // Data is currently saved in memory via Serial
  if (Serial.available() > 0) { // If there is data in Serial
    String s = Serial.readStringUntil('\n'); // Read it until the end of the line
    if (s == "wipe"){ // If its the wipe command,
      wipe(); // wipe the memory
    } else { 
      Serial.println("I received: " + s);
      write(s); // Otherwise save the received data
    }
  }
}