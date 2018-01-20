#ifndef MELEXIS_H_
#define MELEXIS_H_

#include <Arduino.h>
#include <i2cmaster.h>


/*
 *
 *      ---------------------
 *      |                   |
 *      |  A4           A0  |
 *      |        /|\        |
 *      |       / | \       |
 *      |         |         |
 *      |         |         |
 *      |  A6           A2  |
 *      |                   |
 *      --------------------- 
 *        MELEXIS POSITIONS
 * 
 */
void melexisInit();

float melexisTemperature(uint8_t address);

int melexisChangeAddress(uint8_t newAddress);


#endif
