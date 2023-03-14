#include "PressureSensor.h"


PressureSensor::PressureSensor(int pin) {
  this->pin = pin;
  init();
}

void PressureSensor::init() {
  pinMode(pin, INPUT);
}

bool PressureSensor::isEmpty() {
  int sensorReading = analogRead(pin);

  Serial.println(sensorReading);

  return sensorReading < 100;
}
