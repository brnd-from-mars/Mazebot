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

    // disable input buffer for this channel's pin
    // to save power
    if(_channel & 0x8)
        DIDR2 |= (1<<(_channel & 0x7));
    else
        DIDR0 |= (1<<(_channel & 0x7));

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
