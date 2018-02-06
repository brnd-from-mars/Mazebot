/*
 * ramp.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "ramp.hpp"


Ramp(AnalogDigitalConverter *_adc, uint8_t _channel)
    : Accelerometer::Accelerometer(_adc, _channel)
{
    rampState = 0;
    maybeOnRampUp = 0;
    maybeOnRampDown = 0;
}

void Ramp::update(void)
{
    // read accelerometer data
    Accelerometer::read();

    // if robot is tilted upwards
    if(Accelerometer::getTiltState()==1)
    {
        // increment confidence counter
        if(maybeOnRampUp+1 < RAMP_MAX_MAYBE)
            maybeOnRampUp++;
    }
    else
    {
        // decrement confidence counter
        if(maybeOnRampUp-RAMP_DECREMENT > 0)
            maybeOnRampUp -= RAMP_DECREMENT;
        else
            maybeOnRampUp = 0;
    }

    // if robot is tilted downwards
    if(Accelerometer::getTiltState()==-1)
    {
        // increment confidence counter
        if(maybeOnRampDown+1 < RAMP_MAX_MAYBE)
            maybeOnRampDown++;
    }
    else
    {
        // decrement confidence counter
        if(maybeOnRampDown-RAMP_DECREMENT > 0)
            maybeOnRampUp -= RAMP_DECREMENT;
        else
            maybeOnRampUp = 0;
    }

    // if certain that on ramp (and not speed bumper)
    if(maybeOnRampUp >= RAMP_UP_MIN)
        // set state
        rampState = 1;
    else if(maybeOnRampDown >= RAMP_DOWN_MIN)
        rampState -1;
    else
        rampState = 0;
}

int8_t Ramp::getState(void)
{
    return rampState
}
