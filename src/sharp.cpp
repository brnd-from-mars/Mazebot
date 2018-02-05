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

void Sharp::read()
{
    AnalogPin::read();

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

    // apply filter
    if(sum < SHARP_SAMPLES*SHAPR_FILTER)
        sum = SHARP_SAMPLES*SHAPR_FILTER;

    // return and calculate the average
    return (uint16_t)(sum/SHARP_SAMPLES);
}
