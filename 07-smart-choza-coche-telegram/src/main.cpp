#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>

//Datos conexión WiFi
#define WLAN_SSID       "Codemotion_2016"
#define WLAN_PASS       "codemotion2016"


//GPIO sensor de distancia
#define TRIGGER D1
#define ECHO    D2

//GPIO leds
#define RED D6
#define YELLOW D7
#define GREEN  D8

//Umbrales para estimar la distancia
#define FAR    60
#define MEDIUM 30
#define NEAR   15

//Conexión wifi
WiFiClientSecure client;


//Telegram
#define CHATID ""
#define TOKEN ""
String KEYBOARD = "[[\"/disable\", \"/enable\"],[\"/status\"]]";


UniversalTelegramBot telegram(TOKEN, client);

bool enabled;
bool parked;

void connectWiFi();
int time2cm(int time);
void processMessages(int updates);
void sendMessage(String message);


void setup() {
  //El trigger lanzará el pulso,
  //lo configuramos como salida
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);

  //El echo recibirá el pulso
  //lo configuramos como entrada
  pinMode(ECHO, INPUT);

  //Los pines de los leds
  //los configuramos como salida
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  //Apagamos los leds
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  enabled = true;
  parked  = false;

  //Iniciamos el Serial
  Serial.begin(9600);
  delay(200);

  // Conectamos la WiFi y con Adafruit
  connectWiFi();

  //Encendemos el led que lleva la placa integrado
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, enabled);

  sendMessage("Iniciando Smart Choza del Coche");


}

//----------------------------------------------

void loop() {


  int updates = telegram.getUpdates(telegram.last_message_recived + 1);
  while(updates) {
    Serial.println("Nuevo mensaje");
    processMessages(updates);
    updates = telegram.getUpdates(telegram.last_message_recived + 1);
  }



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

  //Calculamos la distancia
  int duration = pulseIn(ECHO, HIGH);
  int distance = time2cm(duration);

  //Si se produce una distancia tan baja, es por algún error
  if (duration == 0)
    distance = FAR;

  if (distance < FAR) {
    Serial.printf("Duración: %d. Distancia: %d\n", duration, distance);
  } else {
    Serial.println("Fuera de rango");
  }

  digitalWrite(GREEN, distance < FAR);
  digitalWrite(YELLOW, distance < MEDIUM);
  digitalWrite(RED, distance < NEAR);

  if (!parked && distance < NEAR) {
    sendMessage("Aparcando");
    parked = true;
  }

  if (parked && distance > FAR) {
    sendMessage("Desaparcando");
    parked = false;
  }
}


//---------------------------------------------

void sendMessage(String message) {
  telegram.sendMessageWithReplyKeyboard(CHATID, "Iniciando Smart Choza del Coche", "", KEYBOARD, true);
}


//----------------------------------------------

void connectWiFi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//----------------------------------------------


void processMessages(int updates) {
  for (int i = 0; i < updates; i++) {
      String text = telegram.messages[i].text;
  }
}


//----------------------------------------------

int time2cm(int time) {
  return (int) round(time * 0.0171232877);
}
