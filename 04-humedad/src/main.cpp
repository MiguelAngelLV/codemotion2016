#include <Arduino.h>
#include "DHT.h"
#define DHT_GPIO D3

DHT dht(DHT_GPIO, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
}


void loop() {

  delay(2*1000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
    Serial.println("Error de lectura");
  else {
    Serial.print("Humedad: ");
    Serial.println(humidity, 2);
    Serial.print("Temperatura: ");
    Serial.println(temperature, 2);
  }
}
