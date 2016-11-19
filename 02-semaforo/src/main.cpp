#include <Arduino.h>

//GPIO leds
#define RED D1
#define YELLOW D2
#define GREEN D3

//----------------------------------------------

void setup() {
  //Los pines de los leds
  //los configuramos como salida
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  //Apagamos los leds
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}

//----------------------------------------------

void loop() {
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  delay(1000);

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);

  delay(1000);

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);

  delay(1000);
}
