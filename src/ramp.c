#include "ramp.h"


void rampInit() {
    rampState = 0;
    maybeOnRampUp = 0;
    maybeOnRampDown = 0;
}

void rampInterrupt() {
    int16_t accel = analogRead(3);

    if(accel > RAMP_UP)
        maybeOnRampUp++;
    else
        maybeOnRampUp = 0;

    if(accel < RAMP_DOWN)
        maybeOnRampDown++;
    else
        maybeOnRampDown = 0;
    
    if(maybeOnRampUp >= 50)
        rampState = 1;
    else if(maybeOnRampDown >= 50)
        rampState = -1;
    else
        rampState = 0;
}
