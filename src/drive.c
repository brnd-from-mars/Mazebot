#include "drive.h"


long correctionStart=0;

int errorI=0;

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

    int targetWalldistance = 240;

    if(entireWall(RIGHT, 150) && entireWall(LEFT, 150)) {
        targetWalldistance = trunc((sharp[3].value + sharp[5].value)/2);
    }

    if(entireWall(RIGHT, 150)) {
        errorP = targetWalldistance - (int)(sharp[3].value);
        errorI = errorI + errorP;
        errorD = sharp[5].value - sharp[3].value;
    } else if(entireWall(LEFT, 150)) {
        errorP = -(targetWalldistance - (int)(sharp[4].value));
        errorI = errorI + errorP;
        errorD = -(sharp[6].value - sharp[4].value);
    }

    motorSetLeftSpeed(baseSpeed + trunc(errorP * kP) + trunc(errorI * kI) + trunc(errorD * kD));
    motorSetRightSpeed(baseSpeed - trunc(errorP * kP) - trunc(errorI * kI) - trunc(errorD * kD));
}

void rotate(int16_t speed) {
    motorSetLeftSpeed(speed);
    motorSetRightSpeed(-speed);
}

bool correctRotationPosition(bool start) {
    if(start) {
        correctionStart = millis();
        return true;
    } else {
        if(millis() < correctionStart+3000) {
            int errorRot=0;
            uint8_t referenceWallsRot=0;

            int errorPos=0;
            uint8_t referenceWallsPos=0;

            if(entireWall(RIGHT, 150)) {
                errorRot+=(sharp[5].value - sharp[3].value);
                referenceWallsRot++;
            }
            if(entireWall(BACK, 150)) {
                errorRot += (sharp[8].value - sharp[7].value);
                referenceWallsRot++;
                errorPos += -(240-(int)(trunc((sharp[7].value + sharp[8].value)/2)));
                referenceWallsPos++;
            }
            if(entireWall(LEFT, 150)) {
                errorRot+=(sharp[4].value - sharp[6].value);
                referenceWallsRot++;
            }
            if(entireWall(FRONT, 150)) {
                errorRot += (sharp[1].value -sharp[2].value);
                referenceWallsRot++;
                errorPos += (240-(int)(trunc((sharp[0].value + sharp[1].value + sharp[2].value)/3)));
                referenceWallsPos++;
            }

            if(referenceWallsPos==0 && referenceWallsRot==0) {
                motorBrake();
                return false;
            }

            int16_t leftSpeed=0;
            int16_t rightSpeed=0;

            if(referenceWallsPos != 0) {
                leftSpeed += (int16_t)(trunc(1.0*errorPos/referenceWallsPos));
                rightSpeed += (int16_t)(trunc(1.0*errorPos/referenceWallsPos));
            }

            if(referenceWallsRot != 0) {
                leftSpeed += (int16_t)(trunc(1.58*errorRot/referenceWallsRot));
                rightSpeed -= (int16_t)(trunc(1.5*errorRot/referenceWallsRot));
            }

            if(abs(leftSpeed)<30 && abs(rightSpeed)<30) {
                motorBrake();
                return false;
            }

            motorSetLeftSpeed(leftSpeed);
            motorSetRightSpeed(rightSpeed);

            return true;
        } else {
            motorBrake();
            return false;
        }
    }
}
