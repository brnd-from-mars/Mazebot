#include "navigation.h"


void navigationInit() {
    enableNavigation = true;
    blackEscaping    = false;
    lastRampState    =  0;
    lastAction       = -1;
    lastRotateState  = -1;
    lastForwardState = -1;
}


void navigate() {

    if(isBlack && !blackEscaping) {
        motorBrake();
        startForwardEnc(-distanceCoveredEnc());
        lastAction = NAVIGATION_ACTION_ESC_BLACK;
        blackEscaping = true;
    }

    if(rampState == 1) {
        drive(140, 0.5, 0.02, 0.0);
        rgbSet(32, 32, 32, 0);
        if(lastRampState != 1) {
            mapSetRamp();
        }

    } else if(rampState == -1) {
        drive(100, 0.5, 0.02, 0.0);
        rgbSet(32, 32, 32, 0);
        if(lastRampState != -1) {
            mapSetRamp();
        }

    } else if(lastRampState != 0) {
        mapFinishRamp();
        motorBrake();
        driveReset();
        lastAction = NAVIGATION_ACTION_DRIVE_RAMP;

    } else {

        if(rotateState == -1 && lastRotateState != rotateState)
            navigationUpdateMap();

        if(forwardState == -1 && lastForwardState != forwardState)
            navigationUpdateMap();

        lastRotateState = rotateState;
        lastForwardState = forwardState;

        if(rotateState != -1)
            processRotate();

        else if(forwardState != -1)
            processForward();

        else {
            
            mapEvaluateScores();

            if(lastScoreInfo.max > 0 && lastScoreInfo.valid) {

                if(lastScoreInfo.adjacentScores[FRONT] == lastScoreInfo.max) {
                    startForwardCM(30);
                    lastAction = NAVIGATION_ACTION_DRIVE_FWD;
                } else if(lastScoreInfo.adjacentScores[RIGHT] == lastScoreInfo.max) {
                    startRotate(90);
                    lastAction = NAVIGATION_ACTION_TURN_RIGHT;
                } else if(lastScoreInfo.adjacentScores[LEFT] == lastScoreInfo.max) {
                    startRotate(-90);
                    lastAction = NAVIGATION_ACTION_TURN_LEFT;
                } else if(lastScoreInfo.adjacentScores[BACK] == lastScoreInfo.max) {
                    startRotate(90);
                    lastAction = NAVIGATION_ACTION_TURN_RIGHT;
                }
            } else {
                lastAction = NAVIGATION_ACTION_NOTHING;
                // serialPrintInt(255);
                // serialPrintNL();
                // mapRestoreFromBackup();
                // mapUpdate();
                // mapSender();
                // encoderReset();
                // driveSMInit();
            }
        }
    }

    lastRampState = rampState;
}

void navigationUpdateMap() {
    blackEscaping = false;
    switch(lastAction) {
    case NAVIGATION_ACTION_NOTHING:
        break;
    case NAVIGATION_ACTION_TURN_RIGHT:
        mapRotate(1);
        break;
    case NAVIGATION_ACTION_TURN_180:
        mapRotate(-2);
        break;
    case NAVIGATION_ACTION_TURN_LEFT:
        mapRotate(-1);
        break;
    case NAVIGATION_ACTION_DRIVE_FWD:
        mapForward(true);
        break;
    case NAVIGATION_ACTION_ESC_BLACK:
        mapSetBlackInFront();
        break;
    case NAVIGATION_ACTION_DRIVE_RAMP:
        break;
    }
}
