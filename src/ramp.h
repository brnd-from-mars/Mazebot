#ifndef RAMP_H_
#define RAMP_H_

#include <Arduino.h>
#include "config.h"
#include "analog.h"
#include "navigation.h"
#include "driveStateMachine.h"
#include "map.h"


uint16_t accel;

bool blockRampUp;

bool blockRampDown;

int8_t rampState;

uint8_t maybeOnRampUp;

uint8_t maybeOnRampDown;

// bool maybeOnRampEndUp;

// bool maybeOnRampEndDown;

// long timeAtPresumption;

void rampInit();

void rampInterrupt();

void rampLoop();

void rampDiscardBecauseRotation();


#endif
