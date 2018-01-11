#ifndef SWITCH_H_
#define SWITCH_H_

#include <Arduino.h>


struct Toggleswitch {
    uint8_t pin;
    bool value;
} toggleswitch[3];

uint8_t nextSwitch;

void switchInit();

void switchInterrupt();


#endif
