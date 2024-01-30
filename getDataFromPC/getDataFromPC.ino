#include <Wire.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

ExternalEEPROM myMem;

int currentNextAddr = 0;

void setup() {
  Wire.begin();
  myMem.setMemoryType(16);
  Serial.begin(9600);

  delay(5000);

  if (myMem.begin() == false)  {
    Serial.print("UH OH");
    while (true);
  }

  currentNextAddr = myMem.read(0);
  while (myMem.read(currentNextAddr) != 0) {
    currentNextAddr += myMem.read(currentNextAddr);
  }

  Serial.println("BOOTED!");
  Serial.println("Current Next Address: " + String(currentNextAddr));
}

void write(String s) {
  int nextAddr = myMem.putString(currentNextAddr + 1, s);
  myMem.write(currentNextAddr,byte(nextAddr-currentNextAddr));
  myMem.write(nextAddr,0);
  currentNextAddr = nextAddr;  
  Serial.println("NextAddr: " + String(currentNextAddr));
}

void wipe() {
  myMem.erase();
  currentNextAddr = 0;
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