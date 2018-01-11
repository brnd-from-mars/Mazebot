#include "drive.h"


int errorI;

void driveInit() {
    errorI = 0;
}

bool entireWall(uint8_t dir, uint16_t maxDistance) {
    switch(dir) {
    case RIGHT:
        return sharp[3].value>maxDistance && sharp[5].value>maxDistance;
    case BACK:
        return sharp[7].value>maxDistance && sharp[8].value>maxDistance;
    case LEFT:
        return sharp[4].value>maxDistance && sharp[6].value>maxDistance;
    case FRONT:
        return sharp[0].value>maxDistance && sharp[1].value>maxDistance && sharp[2].value>maxDistance;
    }
    return false;
}

void driveReset() {
    errorI = 0;
}

void drive(int16_t baseSpeed, float kP, float kI, float kD) {
    int errorP = 0;
    int errorD = 0;

    #define WALLDISTANCE 240

    if(entireWall(RIGHT, 150)) {
        errorP = WALLDISTANCE - (int)(sharp[3].value);
        errorI = errorI + errorP;
        errorD = sharp[5].value - sharp[3].value;
        rgbSet(0, 0, 64, 0);
    } else if(entireWall(LEFT, 150)) {
        errorP = -(WALLDISTANCE - (int)(sharp[4].value));
        errorI = errorI + errorP;
        errorD = -(sharp[6].value - sharp[4].value);
        rgbSet(0, 64, 0, 0);
    } else {
        rgbSet(64, 0, 0, 0);
    }

    #undef WALLDISTANCE

    motorSetLeftSpeed(baseSpeed + trunc(errorP * kP) + trunc(errorI * kI) + trunc(errorD * kD));
    motorSetRightSpeed(baseSpeed - trunc(errorP * kP) - trunc(errorI * kI) - trunc(errorD * kD));
}
