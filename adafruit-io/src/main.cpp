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


//GPIO sensor de distancia
#define TRIGGER D1
#define ECHO    D2

//GPIO sensor DHT
#define DHT_GPIO D3


#define RED D6
#define YELLOW D7
#define GREEN  D8


#define FAR    60
#define MEDIUM 30
#define NEAR   15

//Conexión wifi
WiFiClient client;

//Conexión a Adafruit
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);


//Feeds para publicar información
Adafruit_MQTT_Publish parkedFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "parked");
Adafruit_MQTT_Publish distanceFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "distance");
Adafruit_MQTT_Publish humiditySubFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "humidity");
Adafruit_MQTT_Publish temperatureSubFeed = Adafruit_MQTT_Publish(&mqtt, AIO_FEED "temperature");

//Feed para leer información
Adafruit_MQTT_Subscribe enabledFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_FEED "enabled");


//Sensor de Humedad
DHT dht(DHT_GPIO, DHT11);

bool enabled;
bool parked;


void connect();
int time2cm(int time);
void connectAdafruit();

void setup() {

  //El trigger lanzará el pulso,
  //lo configuramos como salida
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);

  //El echo recibirá el pulso
  //lo configuramos como entrada
  pinMode(ECHO, INPUT);



  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  enabled = true;
  parked  = false;


  Serial.begin(9600);
  delay(200);
  connect();
  connectAdafruit();

  mqtt.subscribe(&enabledFeed);

}

void loop() {

  Adafruit_MQTT_Subscribe * subscription = mqtt.readSubscription(200);

  if (subscription == &enabledFeed)
    if (strcmp((char *)enabledFeed.lastread, "ON") == 0)
        enabled = true;
    else
        enabled = false;


  if (!enabled)
    return;


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

  //Si se produce una distancia tan baja, es por algún error
  if (duration == 0)
    distance = FAR;

  if (distance < FAR) {
    Serial.printf("Duración: %d. Distancia: %d\n", duration, distance);
  }
  else
    Serial.println("Fuera de rango");

  digitalWrite(GREEN, distance < FAR);
  digitalWrite(YELLOW, distance < MEDIUM);
  digitalWrite(RED, distance < NEAR);

  if (!parked && distance < NEAR) {
    parkedFeed.publish("Aparcado");
    parked = true;
  }

  if (parked && distance > FAR) {
    parkedFeed.publish("Desaparcando");
    parked = false;
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
