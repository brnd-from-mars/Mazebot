#include "ramp.h"


void rampInit() {
    accel = 0;
    rampState = 0;
    maybeOnRampUp = 0;
    maybeOnRampDown = 0;
}

void rampInterrupt() {
    accel = analogRead(3);

    if(accel > RAMP_UP && !blackEscaping) {
        if(maybeOnRampUp + 2 < 140)
            maybeOnRampUp += 2;
    } else {
        if(maybeOnRampUp - 10 > 0)
            maybeOnRampUp -= 10;
        else
            maybeOnRampUp = 0;
    }

    if(accel < RAMP_DOWN && !blackEscaping) {
        if(maybeOnRampDown+2 < 180)
            maybeOnRampDown += 2;
    } else {
        if(maybeOnRampDown-7 > 0)
            maybeOnRampDown -= 7;
        else
            maybeOnRampDown = 0;
    }
    
    if(maybeOnRampUp >= 30/** && !blockRampUp*/) {
        if(maybeOnRampUp < 40 && rampState != 1)
            maybeOnRampUp = 40;
        rampState = 1;
    } else if(maybeOnRampDown >= 26/** && !blockRampDown*/) {
        if(maybeOnRampDown < 50 && rampState != -1)
            maybeOnRampDown = 50;
        rampState = -1;
    } else {
        rampState = 0;
    }
}
