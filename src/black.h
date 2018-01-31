#ifndef BLACK_H_
#define BLACK_H_

#include <Arduino.h>


bool blackLedActive;

uint16_t darknessLeft, darknessRight;

void blackInit();

void blackPrepare();

void blackMeasure();


#endif
