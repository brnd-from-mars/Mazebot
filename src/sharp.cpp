/*
 * sharp.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "sharp.hpp"



Sharp::Sharp(AnalogDigitalConverter *_adc, uint8_t _channel)
    : AnalogPin::AnalogPin(_adc, _channel)
{
    // initialize the sample buffer
    for(uint8_t i=0; i<SHARP_SAMPLES; i++)
        lastValues[i] = lastValue;
}

void Sharp::update()
{
    AnalogPin::read();

    if(lastValue>SHARP_MAX)
        return;

    if(lastValue < SHARP_MIN)
        lastValue = SHARP_MIN;

    // shift values
    for(uint8_t i=0; i<SHARP_SAMPLES-1; i++)
         lastValues[i] = lastValues[i+1];

    // append new value
    lastValues[SHARP_SAMPLES-1] = lastValue;
}

uint16_t Sharp::getDistance(void)
{
    uint16_t sum=0;
    
    // calculate sum
    for(uint8_t i=0; i<SHARP_SAMPLES; i++)
        sum += lastValues[i];

    // return and calculate the average
    return (uint16_t)(sum/SHARP_SAMPLES);
}
