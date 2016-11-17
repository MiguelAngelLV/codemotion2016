#include <Arduino.h>

#define TRIGGER D1
#define ECHO    D2

int time2cm(int time) {
  return (int) round(time * 0.0171232877);
}

void setup() {

  Serial.begin(9600);
  //El trigger lanzará el pulso,
  //lo configuramos como salida
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);

  //El echo recibirá el pulso
  //lo configuramos como entrada
  pinMode(ECHO, INPUT);


}

void loop() {
  //Esperamos 4 microsegundos para asegurarnos de que no hay
  //ondas residuales que hagan interferencias
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(4);

  //Mandamos un pulso de 10us
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  int duration = pulseIn(ECHO, HIGH);
  int distance = time2cm(duration);

  if (distance > 100)
    Serial.println("Fuera de rango");
  else
    Serial.printf("Duración: %d. Distancia: %d\n", duration, distance);

  delay(500);
}
