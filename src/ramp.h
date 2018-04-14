#ifndef RAMP_H_
#define RAMP_H_

#include <Arduino.h>
#include "config.h"
#include "navigation.h"
//#include "map.h"


uint16_t accel;

int8_t rampState;

uint8_t maybeOnRampUp;

uint8_t maybeOnRampDown;

void rampInit();

void rampInterrupt();


#endif
