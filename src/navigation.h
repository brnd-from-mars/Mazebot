#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <Arduino.h>
#include "config.h"
#include "drive.h"
#include "encoder.h"
#include "motor.h"


int8_t rotateState;

int8_t forwardState;

bool blockRotateRight;

int targetEncValueRotation;

int targetEncValueForward;

void navigationInit();

void navigationRightWall();

void startRotate(int angle);

void startForward(int distance);

void processRotate();

void processForward();

int distanceCoveredEnc();


#endif
