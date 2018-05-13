#include "ramp.h"


void rampInit() {
    accel = 0;
    rampState = 0;
    maybeOnRampUp = 0;
    maybeOnRampDown = 0;
    // maybeOnRampUp = false;
    // maybeOnRampDown = false;
    // maybeOnRampEndUp = false;
    // maybeOnRampEndDown = false;
    blockRampDown = false;
    blockRampUp = false;
    // timeAtPresumption = -1;
}

void rampInterrupt() {
    // if(timeAtPresumption != -1)
    //     timeAtPresumption++;
}

void rampLoop() {
    accel = analogRead(3);

    // if(accel > RAMP_UP && !blackEscaping && forwardState == 1 && !maybeOnRampUp && rampState == 0 && !sharpBlockedFront(200) && sharp[3].value > 200 && sharp[4].value > 200) {
    //     if(maybeOnRampDown) {
    //         maybeOnRampDown    = false;
    //         maybeOnRampUp      = false;
    //         maybeOnRampEndUp   = false;
    //         maybeOnRampEndDown = false;
    //         timeAtPresumption  = -1;
    //     } else {
    //         maybeOnRampUp      = true;
    //         maybeOnRampDown    = false;
    //         maybeOnRampEndUp   = false;
    //         maybeOnRampEndDown = false;
    //         timeAtPresumption  = 0;
    //     }
    // }

    // if(accel < RAMP_DOWN && !blackEscaping && forwardState == 1 && !maybeOnRampDown && rampState == 0 && !sharpBlockedFront(200) && sharp[3].value > 200 && sharp[4].value > 200) {
    //     if(maybeOnRampUp) {
    //         maybeOnRampDown    = false;
    //         maybeOnRampUp      = false;
    //         maybeOnRampEndUp   = false;
    //         maybeOnRampEndDown = false;
    //         timeAtPresumption  = -1;
    //     } else {
    //         maybeOnRampDown    = true;
    //         maybeOnRampUp      = false;
    //         maybeOnRampEndUp   = false;
    //         maybeOnRampEndDown = false;
    //         timeAtPresumption  = 0;
    //     }
    // }

    // if((sharpBlockedFront(200) || (accel < (RAMP_UP) && accel > (RAMP_DOWN))) && rampState != 0 && !(maybeOnRampEndUp || maybeOnRampDown))  {
    //     maybeOnRampDown = false;
    //     if(rampState == 1)
    //         maybeOnRampEndUp   = true;
    //     else if(rampState == -1)
    //         maybeOnRampEndDown = true;
    //     timeAtPresumption = 0;
    // } else {
    //     maybeOnRampEndDown = false;
    //     maybeOnRampEndUp = false;
    // }

    // if(timeAtPresumption != -1) {
    //     if((timeAtPresumption > 1000) && (maybeOnRampEndUp)) {
    //         if((accel < (RAMP_UP) && accel > (RAMP_DOWN)) || sharpBlockedFront(200)) {
    //             rampState = 0;
    //             maybeOnRampEndUp = false;
    //             timeAtPresumption = -1;
    //         }
    //     } else if((timeAtPresumption > 200) && (maybeOnRampEndDown)) {
    //         if((accel < (RAMP_UP) && accel > (RAMP_DOWN)) || sharpBlockedFront(200)) {
    //             rampState = 0;
    //             maybeOnRampEndDown = false;
    //             timeAtPresumption = -1;
    //         }
    //     } else if((timeAtPresumption > 2000) && maybeOnRampUp && !sharpBlockedFront(200) && sharp[3].value > 200 && sharp[4].value > 200) {
    //         if(accel > RAMP_UP) {
    //             rampState = 1;
    //             maybeOnRampUp = false;
    //             timeAtPresumption = -1;
    //         } else {
    //             rampState = 0;
    //             maybeOnRampUp = false;
    //             timeAtPresumption = -1;
    //         }
    //     } else if((timeAtPresumption > 1000) && maybeOnRampDown && !sharpBlockedFront(150) && sharp[3].value > 200 && sharp[4].value > 200) {
    //         if(accel < RAMP_DOWN) {
    //             rampState = -1;
    //             maybeOnRampDown = false;
    //             timeAtPresumption = -1;
    //         } else {
    //             rampState = 0;
    //             maybeOnRampDown = false;
    //             timeAtPresumption = -1;
    //         }
    //     }
    // }

    // serialPrintInt(accel);
    // serialPrintInt(rampState);
    // serialPrintInt(maybeOnRampDown);
    // serialPrintInt(maybeOnRampUp);
    // serialPrintInt(maybeOnRampEndDown);
    // serialPrintInt(maybeOnRampEndUp);
    // serialPrintNL();

    if(accel > RAMP_UP && !blackEscaping && !sharpBlockedFront(200) && sharp[3].value > 200 && sharp[4].value > 200) {
        if(maybeOnRampUp + 5 < 200)
            maybeOnRampUp += 5;
    } else {
        if(maybeOnRampUp - 3 > 0)
            maybeOnRampUp -= 3;
        else
            maybeOnRampUp = 0;
    }

    if(accel < RAMP_DOWN && !blackEscaping && !sharpBlockedFront(200) && sharp[3].value > 200 && sharp[4].value > 200) {
        if(maybeOnRampDown+3 < 200)
            maybeOnRampDown += 3;
    } else {
        if(maybeOnRampDown-2 > 0)
            maybeOnRampDown -= 2;
        else
            maybeOnRampDown = 0;
    }

    if(maybeOnRampUp >= 60/** && !blockRampUp*/) {
        rampState = 1;
    } else if(maybeOnRampDown >= 60/** && !blockRampDown*/) {
        if(maybeOnRampDown < 40 && rampState != -1)
            maybeOnRampDown = 40;
        rampState = -1;
        blockRampDown = true;
        blockRampUp = true;
    } else {
        rampState = 0;
    }
}

void rampDiscardBecauseRotation() {

    maybeOnRampDown = 0;
    maybeOnRampUp = 0;
    rampState = 0;

}
