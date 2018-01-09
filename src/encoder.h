#ifndef ENCODER_H_
#define ENCODER_H_

#include <Arduino.h>


/*
 *
 *      ---------------------
 *      |                   |
 *      |  3             0  |
 *      |        /|\        |
 *      |       / | \       |
 *      |         |         | // change 2 and 3
 *      |         |         |
 *      |  2             1  |
 *      |                   |
 *      --------------------- 
 *        ENCODER POSITIONS
 * 
 */
struct Encoder {
    uint8_t pin1;
    uint8_t pin2;
    bool forward;
    long value;
} encoder[4];

void encoderReset();

void encoderInit();

void encoder0();

void encoder1();

void encoder2();

void encoder3();


#endif
