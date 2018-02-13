#ifndef DRIVE_H_
#define DRIVE_H_

#include <Arduino.h>
#include "analog.h"
#include "config.h"
#include "motor.h"
#include "rgb.h"
#include <math.h>



void driveReset();

void drive(int16_t baseSpeed, float kP, float kI, float kD);

void rotate(int16_t speed);

bool correctRotationPosition(bool start);


#endif
