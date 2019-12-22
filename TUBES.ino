#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <HCSR04.h>

#define FIREBASE_HOST "smartalarm-38597.firebaseio.com"
#define FIREBASE_AUTH "gFuW1HT0RiVSLEWi4NUGwQ6wuOVKaFJ7gDRNKW0y"
#define WIFI_SSID "JualHPBeliKuota"
#define WIFI_PASSWORD "jualhpluanjing"
#define pinEcho D3
#define pinTrig D4
#define pinBuzzer D1
int durasi, jarak;

void setup() {
  Serial.begin(9600);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.setString("status", "off");
  Firebase.setString("getaran", "off");
}

void loop() {
  String path = "/";
  FirebaseObject object = Firebase.get(path);
  String Status = object.getString("status");
  if(Status.equals("on")){
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);
    durasi = pulseIn(pinEcho, HIGH);
    jarak = (durasi / 2) / 29.1;
    Serial.print("Jarak : ");
    Serial.print(jarak);
    Serial.println(" cm");
    
    String Getar = object.getString("getaran");
    if(Getar.equals("off")){
      if (jarak < 70){
        digitalWrite(pinBuzzer, HIGH);
        delay(100);
        digitalWrite(pinBuzzer, LOW);
        delay(100);
        Firebase.setString("getaran", "on");
      }
      else {
        digitalWrite(pinBuzzer, LOW);
      }
      delay(500); 
    }
  }
}
