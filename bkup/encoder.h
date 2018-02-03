#ifndef ENCODER_H_
#define ENCODER_H_

#include <Arduino.h>


/*
 *
 *      ---------------------
 *      |                   |
 *      |  2             0  |
 *      |        /|\        |
 *      |       / | \       |
 *      |         |         |
 *      |         |         |
 *      |  3             1  |
 *      |                   |
 *      --------------------- 
 *        ENCODER POSITIONS
 * 
 */
struct Encoder {
    uint8_t pin1;
    uint8_t pin2;
    bool forward;
    int value;
} encoder[4];

void encoderReset();

void encoderInit();

void encoder0();

void encoder1();

void encoder2();

void encoder3();


#endif
