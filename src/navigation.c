#include "navigation.h"


void navigationInit() {
    rotateState = -1;
    forwardState = -1;
    blockRotateRight = false;
    enableNavigation = true;
    completed = false;
}

void navigationRightWall() {
    completed = false;
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
        lastRotation = 1;
        break;
    case -90:
        rotateState = 0;
        targetEncValueRotation = -75;
        lastRotation = -1;
        break;
    case -180:
        rotateState = 0;
        targetEncValueRotation = -150;
        lastRotation = -2;
        break;
    }
}

void startForward(int distance) {
    targetEncValueForward = (int)(distance*4.75);
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
            correctRotationPosition(true);
            rotateState = 2;
        } else {
            rgbSet(0, 32, 0, 0);
            rotate((uint16_t)(SIGNUM(targetEncValueRotation)*100));
        }
        break;
    // completed full rotation
    case 2:
        if(!correctRotationPosition(false)) {
            mapCreatorRotate(lastRotation);
            completed = true;
            targetEncValueRotation = 0;
            rotateState = -1;
        }
        rgbSet(0, 0, 32, 0);
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
            driveReset();
            correctRotationPosition(true);
            forwardState = 2;
        } else {
            rgbSet(0, 32, 0, 0);
            drive(100, 0.5, 0.02, 1.0);
        }
        break;
    // completed forward drive
    case 2:
        if(!correctRotationPosition(false)) {
            mapCreatorForward();
            completed = true;
            targetEncValueForward = 0;
            forwardState = -1;
        }
        rgbSet(0, 0, 32, 0);
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
