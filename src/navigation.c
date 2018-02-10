#include "navigation.h"


void navigationInit() {
    enableNavigation = true;
    blackEscaping = false;
    lastAction = -1;
    lastRotateState = -1;
    lastForwardState = -1;
}


void navigate() {
    if(isBlack && !blackEscaping) {
        motorBrake();
        int distanceEnc = distanceCoveredEnc();
        encoderReset();
        serialPrintInt(1000);
        mapFrontFieldBlack();
        serialPrintInt(2000);
        startForwardEnc(-distanceEnc);
        serialPrintInt(3000);
        lastAction = 4;
        serialPrintInt(4000);
        blackEscaping = true;
        serialPrintInt(5000);
    }

    if(rotateState==-1 && lastRotateState!=rotateState)
        navigationUpdateMap(lastAction);

    if(forwardState==-1 && lastForwardState!=forwardState)
        navigationUpdateMap(lastAction);

    lastRotateState = rotateState;
    lastForwardState = forwardState;

    if(rotateState!=-1)
        processRotate();
    else if(forwardState!=-1)
        processForward();
    else {
        AdjacentScores gScores = mapGetAdjacentScores();
        int8_t lScores[4];

        for(uint8_t dir=0; dir<4; dir++) {
            lScores[(dir-heading+3)%4] = gScores.score[dir];
        }

        int8_t maxValue = -2;
        for(uint8_t lDir=0; lDir<4; lDir++)  {
            if(lScores[lDir]>=maxValue) {
                maxValue = lScores[lDir];
            }
        }

        if(maxValue<=0) {
            lastAction = -1;
        } else if(lScores[FRONT]==maxValue) {
            startForwardCM(30);
            lastAction = FRONT;
        } else if(lScores[RIGHT]==maxValue) {
            startRotate(90);
            lastAction = RIGHT;
        } else if(lScores[LEFT]==maxValue) {
            startRotate(-90);
            lastAction = LEFT;
        } else if(lScores[BACK]==maxValue) {
            startRotate(-90);
            lastAction = LEFT;
        }
    }
}

void navigationUpdateMap(int8_t action) {
    blackEscaping = false;
    switch(action) {
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
