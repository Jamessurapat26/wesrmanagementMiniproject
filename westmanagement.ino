/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-hc-sr04-ultrasonic-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include <WiFi.h>
#include <TridentTD_LineNotify.h>
#include <time.h>

const int trigPin = 5;
const int echoPin = 18;
const char *ssid = "IOT Engineering";
const char *password = "coeai123";
const char *thingSpeakAPIKey = "33M4MAGT4QFPOB9O";

#define LINE_TOKEN  "NOFWj20LipsqkrFwlI8o7HpudXo8i9GQkGBAMIT4xvX"   // TOKEN

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  sendToThingSpeak(distanceCm, distanceInch);
  
  if(distanceCm >= 5){
    sendToLine(distanceCm, distanceInch);
  }


  delay(15000);
}

void sendToThingSpeak(float distanceCm, float distanceInch) {
  WiFiClient client;
  const char *server = "api.thingspeak.com";
  String data = "GET /update?api_key=" + String(thingSpeakAPIKey);
  data += "&field1=" + String(distanceCm);
  data += "&field2=" + String(distanceInch);
  data += "\r\n";
  if (client.connect(server, 80)) {
    client.print(data);
    client.stop();
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Failed to connect to ThingSpeak server!");
  }
}

void sendToLine(float distanceCm, float distanceInch){

  LINE.notify("ระดับความสูงเซนติเมตร:" + String(distanceCm));
  LINE.notify("ระดับควาสูงนิ้ว:" + String(distanceInch));

}

