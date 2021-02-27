/*
* Time:20200508
* Author: youngda
* QQ:1217882800
* wechat:19092550573
* QQ�s�G824273231
* ?�إܨҡG�q?�Y�eon��off����??
*/
#include<Arduino.h>
//#include <ESP8266WiFi.h>//�q?�A�[?WIFI�`���
#include <WiFi.h>
#include "PubSubClient.h"//�q?�A�[?MQTT?���

const char* ssid = "CMCC-iupx";//�ק�A�A�����ѥhWIFI�W�r
const char* password = "k7ffd9wp";//�A��WIFI�K?
const char* mqtt_server = "bemfa.com";//�q?�AMQTT�A�U��
const int mqtt_server_port = 9501;//�q?�AMQTT�A�U��
#define ID_MQTT  "43dc97ed0846e284e02843f0eed27301"     //�ק�A�A��Client ID
const char*  topic = "light001";  //�D?�W�r�A�i�b�ڪk������x�ۦ�?�ءA�W?��N


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//������?�Τގ�w��
void turnOnLed();
void turnOffLed();
const int B_led = 2;


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String Mqtt_Buff = "";
  for (int i = 0; i < length; i++) {
    Mqtt_Buff += (char)payload[i];
  }
  Serial.print(Mqtt_Buff);
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (Mqtt_Buff == "on") {//�p�G�����r��on�A�G��
    turnOnLed();//?����?

  } else if (Mqtt_Buff == "off") {//�p�G�����r��off�A�G��
    turnOffLed();//?����?
  }
  Mqtt_Buff = "";
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_MQTT)) {
      Serial.println("connected");

      client.subscribe(topic);//�ק�A�ק�?�A���D?
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(B_led, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);
  digitalWrite(B_led, HIGH);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

//��?���w
void turnOnLed() {
  Serial.println("turn on light");
  digitalWrite(B_led, LOW);
}
//??���w
void turnOffLed() {
    Serial.println("turn off light");
  digitalWrite(B_led, HIGH);
}
