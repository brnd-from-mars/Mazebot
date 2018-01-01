#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>


/*
 *
 *      –––––––––
 *      |2  ^  0|
 *      |   |   |
 *      |3     1|
 *      –––––––––
 *   MOTOR POSITIONS
 * 
 */
struct Motor {
    uint8_t pin1;
    uint8_t pin2;
    uint8_t pwm;
} motor[4];

void motorInit();

void motorSetSpeed(uint8_t motor, int16_t speed);

void motorSetLeftSpeed(int16_t speed);

void motorSetRightSpeed(int16_t speed);

void motorBrake();

#endif
