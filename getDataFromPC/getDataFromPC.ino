#include <Wire.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

ExternalEEPROM myMem;
byte numOfEntries = 0;
byte DataSize = 50;

void setup() {
  Wire.begin();
  myMem.setMemoryType(16);
  Serial.begin(9600);

  delay(5000);

  if (myMem.begin() == false)  {
    Serial.print("UH OH");
    while (true);
  }

  Serial.println("BOOTED!");
  numOfEntries = myMem.read(0);
  Serial.println(" I got numOfEntries: " + String(numOfEntries));
}

void write(String s) {
  int nextAddr = myMem.putString(numOfEntries * DataSize + 10, s);
  numOfEntries++;
  Serial.println("Num of Entries: " + String(numOfEntries));
  Serial.println("NextAddr: " + String(nextAddr));
  myMem.write(0, numOfEntries);
}

void wipe() {
  myMem.erase();
  myMem.write(0, 0);
  numOfEntries = 0;
  Serial.println("Wiped!");
}

void loop() {
  if (Serial.available() > 0) {
    String s = Serial.readStringUntil('\n');
    if (s == "wipe"){
      wipe();
    } else {
      Serial.println("I received: " + s);
      write(s);
    }
  }
}