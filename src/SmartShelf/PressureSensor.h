#ifndef MY_PRESSURESENSOR_H
#define MY_PRESSURESENSOR_H

#include <Arduino.h>

class PressureSensor {
  private: 
    int pin;
  public:
    PressureSensor(int pin);
    void init();
    bool isEmpty();
};

#endif