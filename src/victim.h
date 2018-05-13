#ifndef VICTIM_H_
#define VICTIM_H_

#include <Arduino.h>
#include "config.h"
#include "driveStateMachine.h"
#include "map.h"
#include "melexis.h"
#include "motor.h"
#include "rgb.h"
#include "navigation.h"


int8_t victimState;

uint8_t victimPosition;

uint8_t victimKitdropperLeft;

uint8_t victimKitdropperRight;

uint8_t victimKitdropperi; // i is for the imaginary constant

uint8_t victimCounter;

long victimStart;

void victimInit();

void victimRecognition();

void victimSetVisual(int code);

#endif
