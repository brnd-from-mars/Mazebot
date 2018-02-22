#include "ramp.h"


void rampInit() {
    accel = 0;
    rampState = 0;
    maybeOnRampUp = 0;
    maybeOnRampDown = 0;
}

void rampInterrupt() {
    accel = analogRead(3);

    
    if(accel > RAMP_UP) {
        if(maybeOnRampUp+1 < 200)
            maybeOnRampUp++;
    } else {
        if(maybeOnRampUp-4 > 0)
            maybeOnRampUp -= 4;
        else
            maybeOnRampUp = 0;
    }

    if(accel < RAMP_DOWN) {
        if(maybeOnRampDown+1 < 200)
            maybeOnRampDown++;
    } else {
        if(maybeOnRampDown-4 > 0)
            maybeOnRampDown -= 4;
        else
            maybeOnRampDown = 0;
    }
    
    if(maybeOnRampUp >= 30)
        rampState = 1;
    else if(maybeOnRampDown >= 30)
        rampState = -1;
    else
        rampState = 0;
}
