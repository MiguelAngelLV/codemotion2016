#include <Arduino.h>
#include "DHT.h"

//GPIO sensor DHT
#define DHT_GPIO D3

//Sensor de Humedad
DHT dht(DHT_GPIO, DHT11);

//----------------------------------------------

void setup() {
  //Iniciamos el Serial
  Serial.begin(9600);

  //Iniciamos el sensor
  dht.begin();
}

//----------------------------------------------

void loop() {
  //Esperamos 1 minuto entre actualización
  delay(2*1000);

  //Leemos los valores del sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  //El DHT a veces no lee bien y da errores
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error de lectura");
  } else {
    Serial.print("Humedad: ");
    Serial.println(humidity, 2);
    Serial.print("Temperatura: ");
    Serial.println(temperature, 2);
  }
}
