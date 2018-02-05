/*
 * analogPin.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "analogPin.hpp"


AnalogPin::AnalogPin(AnalogDigitalConverter *_adc, uint8_t _channel)
{
    adc = _adc;
    channel = _channel;
    AnalogPin::read();
}

void AnalogPin::read(void)
{
    adc->setChannel(channel);
    adc->startReading();

    // wait until ready
    while(adc->notReady());

    // copy value
    // important: ADCL should get copied first
    lastValue = ADCL;
    lastValue |= (ADCH & 0x03)<<8;

    adc->finishReading();
}

uint16_t AnalogPin::getLastValue()
{
    return lastValue;
}
