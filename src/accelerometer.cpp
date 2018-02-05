/*
 * accelerometer.cpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "accelerometer.hpp"


Accelerometer::Accelerometer(AnalogDigitalConverter *_adc, uint8_t _channel)
    : AnalogPin::AnalogPin(_adc, _channel)
{
    // initialize the sample buffer
    for(uint8_t i=0; i<SHARP_SAMPLES; i++)
        lastValues[i] = lastValue;
}

void Accelerometer::read(void)
{
    AnalogPin::read();

    // shift values
    for(uint8_t i=0; i<ACCEL_SAMPLES-1; i++)
        lastValues[i] = lastValues[i+1];
    
    // append new value
    lastValues[ACCEL_SAMPLES-1] = lastValue;
}

uint16_t Accelerometer::getAcceleration(void)
{
    uint16_t sum=0;
    
    // calculate sum
    for(uint8_t i=0; i<ACCEL_SAMPLES; i++)
        sum += lastValues[i];

    // return and calculate the average
    return (uint16_t)(sum/ACCEL_SAMPLES);
}

int8_t Accelerometer::getTiltState(void)
{
    // get current filtered acceleration
    uint16_t accel = Accelerometer::getAcceleration();

    // compare with upper/lower limit
    if(accel > TILT_UP_LIMIT)
        return 1;
    else if(accel < TILT_DOWN_LIMIT)
        return -1;
    else
        return 0;
}
