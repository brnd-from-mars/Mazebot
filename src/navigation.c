#include "navigation.h"


void navigationInit() {
    enableNavigation = true;
    blackEscaping = false;
    lastRampState = 0;
    lastAction = -1;
    lastRotateState = -1;
    lastForwardState = -1;
}


void navigate() {
    if(isBlack && !blackEscaping) {
        motorBrake();
        int distanceEnc = distanceCoveredEnc();
        encoderReset();
        // mapFrontFieldBlack();
        startForwardEnc(-distanceEnc);
        lastAction = 4;
        blackEscaping = true;
    }

    if(rampState==1) {
        drive(160, 0.5, 0.02, 1.0);
        rgbSet(32, 32, 32, 0);
        if(lastRampState!=1) {
            // mapSetRamp();
        }
    } else if(rampState==-1) {
        drive(80, 0.5, 0.02, 1.0);
        rgbSet(32, 32, 32, 0);
        if(lastRampState!=-1) {
            // mapSetRamp();
        }
    } else {
        if(lastRampState!=0) {
            // mapFinishRamp();
            driveReset();
        }

        if(rotateState==-1 && lastRotateState!=rotateState)
            navigationUpdateMap();

        if(forwardState==-1 && lastForwardState!=forwardState)
            navigationUpdateMap();

        lastRotateState = rotateState;
        lastForwardState = forwardState;

        if(rotateState!=-1)
            processRotate();
        else if(forwardState!=-1)
            processForward();
        else {
            
            mapEvaluateScores();

            if(lastScoreInfo.max > 0 && lastScoreInfo.valid) {

                if(lastScoreInfo.adjacentScores[FRONT] == lastScoreInfo.max) {
                    startForwardCM(30);
                    lastAction = FRONT;
                } else if(lastScoreInfo.adjacentScores[RIGHT] == lastScoreInfo.max) {
                    startRotate(90);
                    lastAction = RIGHT;
                } else if(lastScoreInfo.adjacentScores[LEFT] == lastScoreInfo.max) {
                    startRotate(-90);
                    lastAction = LEFT;
                } else if(lastScoreInfo.adjacentScores[BACK] == lastScoreInfo.max) {
                    startRotate(90);
                    lastAction = RIGHT;
                }
            } else {
                lastAction = -1;
            }
        }
    }

    lastRampState = rampState;
}

void navigationUpdateMap() {
    blackEscaping = false;
    switch(lastAction) {
    case FRONT:
        mapForward();
        break;
    case LEFT:
        mapRotate(-1);
        break;
    case BACK:
        mapRotate(-2);
        break;
    case RIGHT:
        mapRotate(1);
        break;
    }
}
