#include "motor.h"


void motorInit() {
    motor[0].pin1 = 33;
    motor[0].pin2 = 31;
    motor[0].pwm = 4;
    motor[0].factor = 0.9268225247;

    motor[1].pin1 = 35;
    motor[1].pin2 = 37;
    motor[1].pwm = 5;
    motor[1].factor = 0.9708457497;

    motor[2].pin1 = 45;
    motor[2].pin2 = 43;
    motor[2].pwm = 7;
    motor[2].factor = 0.9415686154;

    motor[3].pin1 = 39;
    motor[3].pin2 = 41;
    motor[3].pwm = 6;
    motor[3].factor = 1.00000000;

    for(uint8_t i=0; i<4; i++) {
        pinMode(motor[i].pin1, OUTPUT);
        pinMode(motor[i].pin2, OUTPUT);
        pinMode(motor[i].pwm, OUTPUT);
    }
}

void motorSetSpeed(uint8_t i, int16_t speed) {
    uint8_t pwm = min(255, round(abs(speed)*motor[i].factor));

    analogWrite(motor[i].pwm, pwm);
    digitalWrite(motor[i].pin1, speed>=0);
    digitalWrite(motor[i].pin2, speed<=0);
}

void motorSetLeftSpeed(int16_t speed) {
    motorSetSpeed(2, speed);
    motorSetSpeed(3, speed);
}

void motorSetRightSpeed(int16_t speed) {
    motorSetSpeed(0, speed);
    motorSetSpeed(1, speed);
}

void motorBrake() {
    for(uint8_t i=0; i<4; i++) {
        motorSetSpeed(i, 0);
    }
}
