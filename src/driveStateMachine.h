#ifndef DRIVESTATEMACHINE_H_
#define DRIVESTATEMACHINE_H_

#include <Arduino.h>
#include "config.h"
#include "drive.h"
#include "encoder.h"
#include "motor.h"


int8_t rotateState;

int8_t forwardState;

int targetEncoderValue;

void driveSMInit();

void startRotate(int angle);

void startForwardEnc(int distance);

void startForwardCM(int distance);

void processRotate();

void processForward();

int distanceCoveredEnc();


#endif
