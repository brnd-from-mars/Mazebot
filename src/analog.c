#include "analog.h"


struct Sharp sharp[9];

uint8_t nextSharp;

void analogInit() {
    sharp[0].pin = A11;
    sharp[1].pin = A0;
    sharp[2].pin = A15;
    sharp[3].pin = A1;
    sharp[4].pin = A4;
    sharp[5].pin = A6;
    sharp[6].pin = A7;
    sharp[7].pin = A5;
    sharp[8].pin = A8;

    for(uint8_t i=0; i<9; i++) {
        sharp[i].value = 0;
        pinMode(sharp[i].pin, INPUT);
    }

    nextSharp = 0;
}

void analogInterrupt() {
    sharp[nextSharp].value = analogRead(sharp[nextSharp].pin);
    if(nextSharp == 8) {
        nextSharp = 0;
    } else {
        nextSharp++;
    }
}
