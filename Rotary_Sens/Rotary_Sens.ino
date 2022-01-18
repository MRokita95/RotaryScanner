#include "RotaryScanner.h"

RotaryScanner scanner1(7,6,12,11,"cm",10);


void setup() {

  Serial.begin(9600);
}

void loop() {

  scanner1.read_command();
 
  delay (100);

  
}
