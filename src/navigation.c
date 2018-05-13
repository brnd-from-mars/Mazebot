#include "navigation.h"


void navigationInit() {
    enableNavigation = true;
    blackEscaping    = false;
    stuckOnBumper    =  0;
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
        drive(160, 0.6, 0.02, 0.0);
        rgbSet(32, 0, 32, 0);
        if(lastRampState != 1 && mapData.currentFloor->id < 8) {
            mapSetRamp();
        }

    } else if(rampState == -1) {
        drive(100, 0.4, 0.00, 0.0);
        rgbSet(32, 0, 32, 0);
        if(lastRampState != -1 && mapData.currentFloor->id < 8) {
            mapSetRamp();
        }

    } else if(lastRampState != 0) {
        if(mapData.currentFloor->id < 8) {
            mapFinishRamp();
        }
        motorBrake();
        driveReset();
        lastAction = NAVIGATION_ACTION_DRIVE_RAMP;

    } else {

        if(rotateState == -1 && lastRotateState != rotateState) {
            navigationUpdateMap();
        }
            

        if(forwardState == -1 && lastForwardState != forwardState) {
            if(moreThanHalfDriven || blackEscaping)
                navigationUpdateMap();
            else
                mapUpdate();
        }

        lastRotateState = rotateState;
        lastForwardState = forwardState;

        if(rotateState != -1)
            processRotate();

        else if(forwardState != -1)
            processForward();

        else {
            
            mapEvaluateScores();

            if(lastScoreInfo.max > 0 && lastScoreInfo.valid) {

                int rotSpeed = ((stuckOnBumper == 0) ? 160 : 200);

                if(lastScoreInfo.adjacentScores[FRONT] == lastScoreInfo.max) {
                    startForwardCM(30);
                    lastAction = NAVIGATION_ACTION_DRIVE_FWD;
                } else if(lastScoreInfo.adjacentScores[RIGHT] == lastScoreInfo.max) {
                    startRotate(90, rotSpeed);
                    lastAction = NAVIGATION_ACTION_TURN_RIGHT;
                } else if(lastScoreInfo.adjacentScores[LEFT] == lastScoreInfo.max) {
                    startRotate(-90, rotSpeed);
                    lastAction = NAVIGATION_ACTION_TURN_LEFT;
                } else if(lastScoreInfo.adjacentScores[BACK] == lastScoreInfo.max) {
                    startRotate(90, rotSpeed);
                    lastAction = NAVIGATION_ACTION_TURN_RIGHT;
                }
            } else {
                lastAction = NAVIGATION_ACTION_NOTHING;
                mapUpdate();
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

int rotErrorCount = 0;

void navigationUpdateMap() {
    blackEscaping = false;
    
    switch(lastAction) {
    case NAVIGATION_ACTION_NOTHING:
        stuckOnBumper = 0;
        break;
    case NAVIGATION_ACTION_TURN_RIGHT:
        if(stuckOnBumper >= 2)
            mapRotate(1, false);
        else {
            if(mapRotate(1, true))
                stuckOnBumper = 0;
            else
                stuckOnBumper++;
        }
        break;
    case NAVIGATION_ACTION_TURN_180:
        if(stuckOnBumper >= 2)
            mapRotate(2, false);
        else {
            if(mapRotate(2, true))
                stuckOnBumper = 0;
            else
                stuckOnBumper++;
        }
        break;
    case NAVIGATION_ACTION_TURN_LEFT:
        if(stuckOnBumper >= 2)
            mapRotate(-1, false);
        else {
            if(mapRotate(-1, true))
                stuckOnBumper = 0;
            else
                stuckOnBumper++;
        }
        break;
    case NAVIGATION_ACTION_DRIVE_FWD:
        mapForward(true);
        stuckOnBumper = 0;
        break;
    case NAVIGATION_ACTION_ESC_BLACK:
        mapSetBlackInFront();
        stuckOnBumper = 0;
        break;
    case NAVIGATION_ACTION_DRIVE_RAMP:
        stuckOnBumper = 0;
        break;
    }
}
