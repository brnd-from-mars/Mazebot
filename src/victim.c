#include "victim.h"


void victimInit() {
    victimState = -1;
    victimSetKitdropper = 0;
}

void victimRecognition() {
    switch(victimState) {
    // not in victim handling process
    case -1:
        // for(uint8_t i=0; i<4; i++) {
        //     if(melexis[i].value > VICTIM_TEMPERATURE) {
        //         victimPosition = i;
        //         victimState = 0;
        //     }
        // }
        if(melexis[1].value > VICTIM_TEMPERATURE) {
            victimPosition = 1;
            victimState = 0;
        }
        if(melexis[3].value > VICTIM_TEMPERATURE) {
            victimPosition = 3;
            victimState = 0;
        }
        break;
    // start victim handling
    case 0:
        motorBrake();
        rgbSet(128, 0, 0, 0);
        enableNavigation = false;
        switch(victimPosition) {
        case 1:
            victimSetKitdropper = 1;
            break;
        case 3:
            victimSetKitdropper = -1;
            break;
        }
        victimStart = millis();
        victimState = 1;
        break;
    // perform victim handling
    case 1:
        if(victimStart+5000 <= millis()) {
            victimState = 2;
        }
        break;
    // finish victim handling
    case 2:
        rgbOff(0);
        enableNavigation = true;
        victimStart = 0;
        victimState = -1;
        break;
    }
}