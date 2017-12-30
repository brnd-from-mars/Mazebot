#ifndef MOTOT_H_
#define MOTOT_H_

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

#endif