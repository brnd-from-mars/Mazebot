#ifndef DRIVESTATEMACHINE_H_
#define DRIVESTATEMACHINE_H_

#include <Arduino.h>
#include "config.h"
#include "analog.h"
#include "drive.h"
#include "encoder.h"
#include "motor.h"
#include "ramp.h"

int obstalceInFrontDetected;

bool moreThanHalfDriven;

int8_t rotateState;

int8_t forwardState;

int targetEncoderValue;

long cameraStart;

long fallbackVerifyTimer;

int rotationSpeed;

void driveSMInit();

void startRotate(int angle, int speed);

void startForwardEnc(int distance);

void startForwardCM(int distance);

void processRotate();

void processForward();

void sendScanningCommand();

int distanceCoveredEnc();

int getRotationProcess();

int getForwardProcess(float threshold);


#endif
