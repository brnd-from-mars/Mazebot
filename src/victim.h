#ifndef VICTIM_H_
#define VICTIM_H_

#include <Arduino.h>
#include "config.h"
#include "melexis.h"
#include "navigation.h"


int8_t victimState;

uint8_t victimPosition;

long victimStart;

void victimInit();

void victimRecognition();


#endif
