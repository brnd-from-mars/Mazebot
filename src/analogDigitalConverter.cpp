/*
 * analogDigitalConverter.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "analogDigitalConverter.hpp"


AnalogDigitalConverter::AnalogDigitalConverter()
{
    // set Vcc pin to analog reference
    ADMUX |= (1<<REFS0);

    // enable the ADC
    ADCSRA |= (1<<ADEN);

    // set prescale to 128 -> 125kHz sampling
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void AnalogDigitalConverter::setChannel(uint8_t _channel)
{
    // reset MUX bits
    ADMUX &= 0xE0;
    ADCSRB &= 0xF7;

    // set MUX to choosen channel
    ADMUX |= (_channel & 0x07)<<MUX0;
    ADCSRB |= ((_channel & 0x08)>>3)<<MUX5;

    currentChannel = _channel;
}

uint8_t AnalogDigitalConverter::getChannel(void)
{
    return currentChannel;
}

void AnalogDigitalConverter::startReading(void)
{
    // disable interrupts
    SREG &= ~(1<<SREG_I);
    // initiate sampling process
    ADCSRA |= (1<<ADSC);
}

bool AnalogDigitalConverter::notReady(void)
{
    // if sampling process has finished
    if(ADCSRA & (1<<ADIF))
    {
        ADCSRA &= ~(1<<ADIF);
        return false;
    }
    else
        return true;
}

void AnalogDigitalConverter::finishReading(void)
{
    // enable interrupts
    SREG |= (1<<SREG_I);
}
