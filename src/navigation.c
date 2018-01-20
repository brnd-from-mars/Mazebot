#include "navigation.h"


int8_t rotateState;

int8_t forwardState;

bool blockRotateRight;

int targetEncValueRotation;

int targetEncValueForward;

void navigationInit() {
    rotateState = -1;
    forwardState = -1;
    blockRotateRight = false;
}

void navigationRightWall() {
    if(rotateState != -1) {
        processRotate();
    } else if (forwardState != -1) {
        processForward();
    } else {
        if(!entireWall(RIGHT, 150) && !blockRotateRight) {
            startRotate(90);
            blockRotateRight = true;
        } else if(entireWall(FRONT, 150)) {
            if(entireWall(LEFT, 150)) {
                startRotate(-180);
            } else {
                startRotate(-90);
            }
            blockRotateRight = false;
        } else {
            startForward(30);
            blockRotateRight = false;
        }
    }
}

void startRotate(int angle) {
    switch(angle) {
    case 90:
        rotateState = 0;
        targetEncValueRotation = 75;
        break;
    case -90:
        rotateState = 0;
        targetEncValueRotation = -75;
        break;
    case -180:
        rotateState = 0;
        targetEncValueRotation = -150;
        break;
    }
}

void startForward(int distance) {
    targetEncValueForward = (int)(distance*4.5);
    forwardState = 0;
}

void processRotate() {
    switch(rotateState) {
    // do not perform rotation, processRotate shouldn't get called
    case -1:
        motorBrake();
        targetEncValueRotation = 0;
        break;
    // starting performing rotation
    case 0:
        motorBrake();
        encoderReset();
        rotateState = 1;
        break;
    // performing rotation
    case 1:
        if(distanceCoveredEnc() >= abs(targetEncValueRotation)) {
            rotateState = 2;
        } else {
            rotate((uint16_t)(SIGNUM(targetEncValueRotation)*100));
        }
        break;
    // completed full rotation
    case 2:
        motorBrake();
        rotateState = -1;
        targetEncValueRotation = 0;
        delay(300);
        break;
    }
}

void processForward() {
    switch(forwardState) {
    // do not perform driving, processForward shouldn't get called
    case -1:
        motorBrake();
        targetEncValueForward = 0;
        break;
    // starting forward drive
    case 0:
        motorBrake();
        encoderReset();
        forwardState = 1;
        break;
    // performing forward drive
    case 1:
        if(distanceCoveredEnc() >= abs(targetEncValueForward)) {
            forwardState = 2;
        } else {
            drive(100, 0.5, 0.02, 1.0);
        }
        break;
    // completed forward drive
    case 2:
        motorBrake();
        forwardState = -1;
        targetEncValueForward = 0;
        driveReset();
        delay(300);
        break;
    }
}

int distanceCoveredEnc() {
    int avg;
    for(uint8_t i=0; i<4; i++) {
        avg += abs(encoder[0].value);
    }
    return round(avg/4);
}
