#include "victim.h"


void victimInit() {
    victimState = -1;
    victimKitdropperLeft = 0;
    victimKitdropperRight = 0;
    victimKitdropperi = 0;
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
                victimCounter = 100;
            }
            TIMER_START;
            break;
        // start victim handling
        case 1:
            mapCreateVictim(((victimPosition == 0) ? RIGHT : LEFT), 3);
            rgbSet(128, 0, 0, 0);
            if(victimPosition == 0)
                victimKitdropperRight = 1;
            if(victimPosition == 1)
                victimKitdropperLeft = 1;
            victimStart = millis();
            victimState = 2;
            break;
        // perform victim handling
        case 2:
            if(victimStart+5000 <= millis())
                victimState = 3;
            rgbSet(128, 0, 0, 0);
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

void victimSetVisual(int code) {

    uint8_t* kitdropper = NULL;

    if(((code & 0x0F0) == 0x000) & (victimKitdropperLeft == 0)) {
        kitdropper = &victimKitdropperLeft;
    }

    if(((code & 0x0F0) == 0x010) & (victimKitdropperRight == 0)) {
        kitdropper = &victimKitdropperRight;
    }

    if(kitdropper == NULL)
        return;

    switch(code & 0x00F) {
    case 0:
        *kitdropper = 2;
        break;
    case 1:
        *kitdropper = 1;
        break;
    case 2:
        victimKitdropperi = 1;
        break;
    case 3:
        *kitdropper = 0;
        break;
    }

}
