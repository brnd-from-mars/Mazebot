#include "motot.h"

struct Motor motor[4];

void motorInit() {
    motor[0].pin1 = 31;
    motor[0].pin2 = 33;
    motor[0].pwm = 4;

    motor[1].pin1 = 37;
    motor[1].pin2 = 33;
    motor[1].pwm = 5;

    motor[2].pin1 = 41;
    motor[2].pin2 = 39;
    motor[2].pwm = 6;

    motor[3].pin1 = 43;
    motor[3].pin2 = 45;
    motor[3].pwm = 7;

    for(uint8_t i=0; i<4; i++) {
        pinMode(motor[i].pin1, OUTPUT);
        pinMode(motor[i].pin2, OUTPUT);
        pinMode(motor[i].pwm, OUTPUT);
    }
}