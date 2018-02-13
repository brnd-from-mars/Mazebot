#include "drive.h"


long correctionStart=0;

int errorI=0;

void driveReset() {
    errorI = 0;
}

void drive(int16_t baseSpeed, float kP, float kI, float kD) {
    int errorP = 0;
    int errorD = 0;

    int targetWalldistance = 250;

    if(entireWall(RIGHT, 150) && entireWall(LEFT, 150)) {
        targetWalldistance = trunc((sharp[3].value + sharp[5].value)/2);
    }

    if(entireWall(RIGHT, 200)) {
        errorP = targetWalldistance - (int)(sharp[3].value);
        errorI = errorI + errorP;
        errorD = sharp[5].value - sharp[3].value;
    } else if(entireWall(LEFT, 200)) {
        errorP = -(targetWalldistance - (int)(sharp[4].value));
        errorI = errorI + errorP;
        errorD = -(sharp[6].value - sharp[4].value);
    } else {
        errorI = 0;
    }

    motorSetLeftSpeed(baseSpeed + trunc(errorP * kP) + trunc(errorI * kI) + trunc(errorD * kD));
    motorSetRightSpeed(baseSpeed - trunc(errorP * kP) - trunc(errorI * kI) - trunc(errorD * kD));
}

void rotate(int16_t speed) {
    motorSetLeftSpeed(speed);
    motorSetRightSpeed(-speed);
}

bool correctRotationPosition(bool start) {
    uint16_t walllimit = 120;

    if(start) {
        correctionStart = millis();
        return true;
    } else {
        if(millis() < correctionStart+3000) {
            int errorRot=0;
            uint8_t referenceWallsRot=0;

            int errorPos=0;
            uint8_t referenceWallsPos=0;

            if(entireWall(RIGHT, walllimit)) {
                errorRot+=(sharp[5].value - sharp[3].value);
                referenceWallsRot++;
            }
            if(entireWall(BACK, walllimit)) {
                errorRot += (sharp[8].value - sharp[7].value);
                referenceWallsRot++;
                errorPos += -(250-(int)(trunc((sharp[7].value + sharp[8].value)/2)));
                referenceWallsPos++;
            }
            if(entireWall(LEFT, walllimit)) {
                errorRot+=(sharp[4].value - sharp[6].value);
                referenceWallsRot++;
            }
            if(entireWall(FRONT, walllimit)) {
                errorRot += (sharp[1].value -sharp[2].value);
                referenceWallsRot++;
                errorPos += (250-(int)(trunc((sharp[0].value + sharp[1].value + sharp[2].value)/3)));
                referenceWallsPos++;
            }

            if(referenceWallsPos==0 && referenceWallsRot==0) {
                motorBrake();
                return false;
            }

            int16_t leftSpeed=0;
            int16_t rightSpeed=0;

            if(referenceWallsPos != 0) {
                leftSpeed += (int16_t)(trunc(1.2*errorPos/referenceWallsPos));
                rightSpeed += (int16_t)(trunc(1.2*errorPos/referenceWallsPos));
            }

            if(referenceWallsRot != 0) {
                leftSpeed += (int16_t)(trunc(1.6*errorRot/referenceWallsRot));
                rightSpeed -= (int16_t)(trunc(1.6*errorRot/referenceWallsRot));
            }

            if(abs(leftSpeed)<40 && abs(rightSpeed)<40) {
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
