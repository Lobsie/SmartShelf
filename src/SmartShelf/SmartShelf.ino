#include "Firebase_Arduino_WiFiNINA.h"

#include "Led.h"
#include "PressureSensor.h"

// Define pins for leds
#define redLightPinOne 12     // Arduino pin D12
#define redLightPinTwo 11     // Arduino pin D11
#define redLightPinThree 10   // Arduino pin D10
#define greenLightPin 9       // Arduino pin D9
#define greenLightPinTwo 8    // Arduino pin D8
#define greenLightPinThree 7  // Arduino pin D7

// Define pins for sensors
#define sensorOnePin A0
#define sensorTwoPin A1
#define sensorThreePin A2

// FireBase
#define DB_URL "smartshelf-d9881-default-rtdb.europe-west1.firebasedatabase.app"
#define DATABASE_SECRET "bnd5Bk3rDJvXn3z5kL2CjKPHn91vQlMc7HlNZ57i"
#define API_KEY "AIzaSyAoGbaTufIqsk7NrfWPihOwPppSH2YivbY"

// Define WiFi
// #define WifiSsid "Palaganas Duyck WiFi"
// #define WifiPassword "L1mb0m@N"

#define WifiSsid "GijsHotspot"
#define WifiPassword "l1mb0m4n"

// Declare leds
Led redLedOne(redLightPinOne);
Led redLedTwo(redLightPinTwo);
Led redLedThree(redLightPinThree);

Led greenLedOne(greenLightPin);
Led greenLedTwo(greenLightPinTwo);
Led greenLedThree(greenLightPinThree);

// Declare sensors
PressureSensor sensorOne(sensorOnePin);
PressureSensor sensorTwo(sensorTwoPin);
PressureSensor sensorThree(sensorThreePin);

// Add timer for push notification
int timer = 0;
int timerTwo = 0;
int timerThree = 0;

// Firebase dataobject
FirebaseData fbdo;

String path = "/readings";

void setup() {
  // put your setup code here, to run once

  Serial.begin(9600);

  delay(100);
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WifiSsid, WifiPassword);
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Provide the autntication data
  Firebase.begin(DB_URL, DATABASE_SECRET, WifiSsid, WifiPassword);
  Firebase.reconnectWiFi(true);

  // if (!Firebase.beginStream(stream, path)) {
  //   Serial.println("Can't connect to stream, " + stream.errorReason());
  // }
}

void loop() {
  // put your main code here, to run repeatedly:


  if (sensorOne.isEmpty()) {
    timer = timer >= 0 ? timer += 1 : 0;
  }

  if (sensorTwo.isEmpty()) {
    timerTwo = timerTwo >= 0 ? timerTwo += 1 : 0;
  }

  if (sensorThree.isEmpty()) {
    timerThree = timerThree >= 0 ? timerThree += 1 : 0;
  }

  // LOW is ON en HIGH is OFF
  if (!sensorOne.isEmpty()) {
    timer = 0;
    Serial.println("Full");
    redLedOne.off();
    greenLedOne.on();
  }

  if (!sensorTwo.isEmpty()) {
    timerTwo = 0;
    Serial.println("Full");
    redLedTwo.off();
    greenLedTwo.on();
  }

  if (!sensorThree.isEmpty()) {
    timerThree = 0;
    Serial.println("Full");
    redLedThree.off();
    greenLedThree.on();
  }

  if (timer > 20) {
    redLedOne.on();  // Send push notification here when timer hits 20 seconds or how long before it needs to send a notification
    greenLedOne.off();
  }

  if (timerTwo > 20) {
    redLedTwo.on();  // Send push notification here when timer hits 20 seconds or how long before it needs to send a notification
    greenLedTwo.off();
  }

  if (timerThree > 20) {
    redLedThree.on();  // Send push notification here when timer hits 20 seconds or how long before it needs to send a notification
    greenLedThree.off();
  }

  String jsonDataOne = "{\"Name\": \"SensorOne\", \"Empty\":" + String(sensorOne.isEmpty()) + ", \"TimeEmpty\":" + timer + "}";

  if (Firebase.updateNode(fbdo, "/sensorOne", jsonDataOne)) {
    Serial.println("OK, sent: " + jsonDataOne);
  } else {
    Serial.println("Error: " + fbdo.errorReason());
  }

  fbdo.clear();

  String jsonDataTwo = "{\"Name\": \"SensorTwo\", \"Empty\":" + String(sensorTwo.isEmpty()) + ", \"TimeEmpty\":" + timerTwo + "}";

  if (Firebase.updateNode(fbdo, "/sensorTwo", jsonDataTwo)) {
    Serial.println("OK, sent: " + jsonDataTwo);
  } else {
    Serial.println("Error: " + fbdo.errorReason());
  }

  fbdo.clear();

  String jsonDataThree = "{\"Name\": \"SensorThree\", \"Empty\":" + String(sensorThree.isEmpty()) + ", \"TimeEmpty\":" + timerThree + "}";

  if (Firebase.updateNode(fbdo, "/sensorThree", jsonDataThree)) {
    Serial.println("OK, sent: " + jsonDataThree);
  } else {
    Serial.println("Error: " + fbdo.errorReason());
  }

  fbdo.clear();

  delay(1000);
}
