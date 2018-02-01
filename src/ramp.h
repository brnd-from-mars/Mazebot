#ifndef RAMP_H_
#define RAMP_H_

#include <Arduino.h>
#include "config.h"


int8_t rampState;

uint8_t maybeOnRampUp;

uint8_t maybeOnRampDown;

void rampInit();

void rampInterrupt();


#endif
