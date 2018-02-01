#include "navigation.h"


void navigationInit() {
    rotateState = -1;
    forwardState = -1;
    blockRotateRight = false;
    enableNavigation = true;
}

void navigationRightWall() {
    if(rotateState != -1) {
        processRotate();
    } else if (forwardState != -1) {
        processForward();
    } else {
        if(blackEscaping) {
            startRotate(-90);
            blockRotateRight = true;
        } else if(!entireWall(RIGHT, 150) && !blockRotateRight && mapAllowRightTurn()) {
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
        targetEncValueRotation = 70;
        lastRotation = 1;
        break;
    case -90:
        rotateState = 0;
        targetEncValueRotation = -70;
        lastRotation = -1;
        break;
    case -180:
        rotateState = 0;
        targetEncValueRotation = -140;
        lastRotation = -2;
        break;
    }
}

void startForward(int distance) {
    targetEncValueForward = (int)(distance*4.50);
    forwardState = 0;
}

void startForwardByEncoder(int distance) {
    targetEncValueForward = distance;
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
            blackEscaping = false;
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
        if(((darknessLeft > 900) || (darknessRight > 900)) && !blackEscaping) {
            driveReset();
            blackEscaping = true;
            mapCreatorBlackField();
            startForwardByEncoder(-distanceCoveredEnc());
        } else if(distanceCoveredEnc() >= abs(targetEncValueForward)) {
            driveReset();
            correctRotationPosition(true);
            forwardState = 2;
        } else {
            rgbSet(0, 32, 0, 0);
            drive(SIGNUM(targetEncValueForward)*100, 0.5, 0.02, 1.0);
        }
        break;
    // completed forward drive
    case 2:
        if(!correctRotationPosition(false)) {
            if(!blackEscaping)
                mapCreatorForward();
            targetEncValueForward = 0;
            forwardState = -1;
        }
        rgbSet(0, 0, 32, 0);
        break;
    }
}

int distanceCoveredEnc() {
    int avg=0;
    for(uint8_t i=0; i<4; i++)
        avg += abs(encoder[i].value);

    return round(avg/4);
}
