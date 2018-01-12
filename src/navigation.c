#include "navigation.h"


int8_t rotateState=-1;

int8_t forwardState=-1;

bool blockRotateRight=false;

int targetEncValueRotation=0;

int targetEncValueForward=0;

void navigationRightWall() {
    if(rotateState != -1) {
        rgbSet(0, 64, 0, 0);
        processRotate();
    } else if (forwardState != -1) {
        rgbSet(0, 0, 64, 0);
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
        } else {
            startForward(30);
        }
    }
}

void startRotate(int angle) {
    switch(angle) {
    case 90:
        rotateState = 0;
        targetEncValueRotation = 30;
        break;
    case -90:
        rotateState = 0;
        targetEncValueRotation = -30;
        break;
    case -180:
        rotateState = 0;
        targetEncValueRotation = -60;
        break;
    }
}

void startForward(int distance) {
    targetEncValueForward = distance / 30 * 40;
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
        if(abs(encoder[1].value) +
           abs(encoder[2].value) +
           abs(encoder[3].value) +
           abs(encoder[4].value) >= 4*abs(targetEncValueRotation)) {
            rotateState = 2;
        } else {
            rotate(SIGNUM(targetEncValueRotation) * 50);
        }
        break;
    // completed full rotation
    case 2:
        motorBrake();
        rotateState = -1;
        targetEncValueRotation = 0;
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
        if(abs(encoder[0].value) +
           abs(encoder[1].value) +
           abs(encoder[2].value) +
           abs(encoder[3].value) >= abs(targetEncValueForward)) {
            rotateState = 2;
        } else {
            drive(100, 0.5, 0.02, 1.0);
        }
    }
}
