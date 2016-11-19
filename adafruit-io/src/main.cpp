#include <Arduino.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "Codemotion_2016"
#define WLAN_PASS       "codemotion2016"


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    ""
#define AIO_KEY         ""
#define AIO_FEED        AIO_USERNAME "/feeds/"


//GPIO sensor DHT
#define DHT_GPIO D3


//Conexión wifi
WiFiClient client;

//Conexión a Adafruit
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);


//Feeds para publicar información
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "humidity");
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "temperature");


//Sensor de Humedad
DHT dht(DHT_GPIO, DHT11);


void connect();
void connectAdafruit();

void setup() {

  Serial.begin(9600);
  delay(200);
  connect();
  connectAdafruit();

  dht.begin();

}

void loop() {

  delay(60*1000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
    Serial.println("Error de lectura");
  else {
    Serial.print("Humedad: ");
    Serial.println(humidity, 2);
    Serial.print("Temperatura: ");
    Serial.println(temperature, 2);

    humidityFeed.publish(humidity, 2);
    temperatureFeed.publish(temperature, 2);
  }


}


void connect() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void connectAdafruit() {

  if (mqtt.connected())
    return;

  Serial.print("Connecting to MQTT... ");
  while (mqtt.connect() != 0) {
       Serial.println("Error. Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
  }

}


int time2cm(int time) {
  return (int) round(time * 0.0171232877);
}
