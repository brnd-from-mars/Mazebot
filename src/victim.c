#include "victim.h"


void victimInit() {
    victimState = -1;
    victimSetKitdropper = 0;
    victimCounter = 0;
}

void victimRecognition() {
    if(victimCounter != 0) {
        victimCounter--;
    } else {
        switch(victimState) {
        // not in victim handling process
        case -1:
            for(uint8_t i=0; i<2; i++) {
                if(melexis[i].value >= VICTIM_TEMPERATURE) {
                    victimPosition = i;
                    victimState = 0;
                    enableNavigation = false;
                }
            }
            break;
        // verify recognized victim
        case 0:
            TIMER_STOP;
            motorBrake();
            short dir = ((victimPosition == 0) ? RIGHT : LEFT);
            if(mapGetVictimType(dir) == -1)
                victimState = 1;
            else {
                enableNavigation = true;
                victimStart = 0;
                victimState = -1;
            }
            TIMER_START;
            break;
        // start victim handling
        case 1:
            mapCreateVictim(((victimPosition == 0) ? RIGHT : LEFT), 3);
            rgbSet(128, 0, 0, 0);
            if(victimPosition == 0)
                victimSetKitdropper = 1;
            if(victimPosition == 1)
                victimSetKitdropper = -1;
            victimStart = millis();
            victimState = 2;
            break;
        // perform victim handling
        case 2:
            if(victimStart+5000 <= millis())
                victimState = 3;
            break;
        // finish victim handling
        case 3:
            rgbOff(0);
            enableNavigation = true;
            victimStart = 0;
            victimState = -1;
            victimCounter = 100;
            break;
        }
    }
}
