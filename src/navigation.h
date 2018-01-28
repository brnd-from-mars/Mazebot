#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <Arduino.h>
#include "config.h"
#include "drive.h"
#include "encoder.h"
#include "map.h"
#include "motor.h"


bool completed;

int8_t rotateState;

int8_t forwardState;

int8_t lastRotation;

bool blockRotateRight;

bool enableNavigation;

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
