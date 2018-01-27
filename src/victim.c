#include "victim.h"


void victimInit() {
    victimState = -1;
}

void victimRecognition() {
    switch(victimState) {
    // not in victim handling process
    case -1:
        for(uint8_t i=0; i<4; i++) {
            if(melexis[i].value > VICTIM_TEMPERATURE) {
                victimPosition = i;
            }
        }
        victimState = 0;
        break;
    // start victim handling
    case 0:
        motorBrake();
        rgbSet(128, 0, 0, 0);
        enableNavigation = false;
        victimStart = millis();
        victimState = 1;
        break;
    // perform victim handling
    case 1:
        if(victimStart+5000 > millis()) {
            victimState = 0;
            //kitdropperMakeSomeStuff()
        }
        break;
    // finish victim handling
    case 2:
        rgbSet(0, 0, 0, 0);
        enableNavigation = true;
        victimStart = 0;
        victimState = -1;
        break;
    }
}