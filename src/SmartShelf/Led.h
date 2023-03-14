#ifndef MY_LED_H
#define MY_LED_H

#include <Arduino.h>

class Led {
  private:
    int pin;
  public:
    Led(int pin);
    void init();
    void on();
    void off();
};

#endif