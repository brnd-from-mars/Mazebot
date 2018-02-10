#ifndef NAVIGATION_H_
#define NAVIGATION_H_


#include <Arduino.h>
#include "config.h"
#include "map.h"
#include "driveStateMachine.h"



bool enableNavigation;

int8_t lastAction;

int8_t lastRotateState;

int8_t lastForwardState;

void navigationInit();

void navigate();

void navigationUpdateMap(int8_t action);


#endif
