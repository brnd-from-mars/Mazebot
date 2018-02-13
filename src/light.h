#ifndef LIGHT_H_
#define LIGHT_H_

#include <Arduino.h>
#include "config.h"


bool hiPoLedActive;

uint16_t darknessLeft, darknessRight;

bool isBlack, isSilver;

void lightInit();

void lightPrepare();

void lightMeasure();


#endif
