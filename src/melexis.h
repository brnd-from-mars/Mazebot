#ifndef MELEXIS_H_
#define MELEXIS_H_

#include <Arduino.h>
#include <i2cmaster.h>


/*
 *
 *      ---------------------
 *      |                   |
 *      |  A2           A4  |
 *      |        /|\        |
 *      |       / | \       |
 *      |         |         |
 *      |         |         |
 *      |  A0           A6  |
 *      |                   |
 *      --------------------- 
 *        MELEXIS POSITIONS
 * 
 */
struct Melexis {
    uint8_t address;
    float value;
} melexis[2];

uint8_t nextMelexis;

void melexisInit();

void melexisInterrupt();

float melexisVerify(uint8_t i);

float melexisTemperature(uint8_t address);

int melexisChangeAddress(uint8_t newAddress);


#endif
