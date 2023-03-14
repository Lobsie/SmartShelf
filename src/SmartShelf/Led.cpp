#include "Led.h"

Led::Led(int pin) {
  this->pin = pin;
  init();    
}

void Led::init() {
  pinMode(pin, OUTPUT);
  off();
}

void Led::on() {
  digitalWrite(pin, LOW);
}

void Led::off() {
  digitalWrite(pin, HIGH);
}