#include "switch.h"


struct Toggleswitch toggleswitch[3];

uint8_t nextSwitch;

void switchInit() {
    toggleswitch[0].pin = 40;
    toggleswitch[1].pin = 42;
    toggleswitch[2].pin = 44;

    for(uint8_t i=0; i<3; i++) {
        pinMode(toggleswitch[i].pin, INPUT);
        toggleswitch[i].value = digitalRead(toggleswitch[i].pin);
    }

    nextSwitch = 0;
}

void switchInterrupt() {
    toggleswitch[nextSwitch].value = digitalRead(toggleswitch[nextSwitch].pin);
    if(nextSwitch == 2) {
        nextSwitch = 0;
    } else {
        nextSwitch++;
    }
}
