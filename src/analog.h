#ifndef ANALOG_H_
#define ANALOG_H_

#include <Arduino.h>
#include "config.h"


/*
 *
 *      ---------------------
 *      |    2    0    1    |
 *      |                   |
 *      |  4     /|\     3  |
 *      |       / | \       |
 *      |         |         |
 *      |  6      |      5  |
 *      |                   |
 *      |     8       7     |
 *      --------------------- 
 *         SHARP POSITIONS
 * 
 */
struct Sharp {
    uint8_t pin;
    uint16_t value;
} sharp[9];

uint8_t nextSharp;

void analogInit();

void analogInterrupt();

bool entireWall(uint8_t dir, uint16_t maxDistance);

uint8_t getWallData(uint8_t hdg);


#endif
