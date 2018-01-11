#ifndef DRIVE_H_
#define DRIVE_H_

#include <Arduino.h>
#include "analog.h"
#include "config.h"
#include "motor.h"
#include "rgb.h"


void driveInit();

bool entireWall(uint8_t dir, uint16_t maxDistance);

void driveReset();

void drive(int16_t baseSpeed, float kP, float kI, float kD);


#endif
