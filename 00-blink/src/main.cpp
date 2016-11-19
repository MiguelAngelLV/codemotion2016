/*
 * Blink
 * Turns on the onboard LED on for one second, then off for one second, repeatedly.
 * This uses delay() to pause between LED toggles.
 */

#include <Arduino.h>

void setup() {
  pinMode(D5, OUTPUT);  // initialize onboard LED as output
}

void loop() {
  digitalWrite(D5, HIGH);  // turn on LED with voltage HIGH
  delay(200);                      // wait one second
  digitalWrite(D5, LOW);   // turn off LED with voltage LOW
  delay(200);                      // wait one second
}
