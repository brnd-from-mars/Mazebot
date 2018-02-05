/*
 * timer.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "timer.hpp"


Timer::Timer(uint16_t _prescaler, uint16_t _top, uint8_t _loopMax)
{
    // reset timer/counter control registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    // reset timer interrupt mask
    TIMSK1 = 0;

    // set timer 1 to clear timer on compare mode
    TCCR1B |= (1<<WGM12);

    // activate interrupt for compare A
    TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);

    // set prescaler and resolution -> frequency
    oldPrescaler = 0;
    Timer::setPrescaler(_prescaler);
    Timer::setTop(_top);

    // set maximum loop count
    loopMax = _loopMax;
    loopCnt = 0;
}

void Timer::setPrescaler(uint16_t _prescaler)
{
    prescaler = _prescaler;

    // reset prescale settings
    TCCR1B &= 0xF8;
    
    switch(_prescaler)
    {
    case 1:
        TCCR1B |= (1<<CS10);
        break;
    case 8:
        TCCR1B |= (1<<CS11);
        break;
    case 64:
        TCCR1B |= (1<<CS11) | (1<<CS10);
        break;
    case 256:
        TCCR1B |= (1<<CS12);
        break;
    case 1024:
        TCCR1B |= (1<<CS12) | (1<<CS10);
        break;
    default:
        prescaler = 0;
        break;
    }
}

void Timer::setTop(uint16_t _top)
{
    top = _top;

    // set output compare register which indicates the TOP value
    OCR1AH = (top & 0xFF00) >> 8;
    OCR1AL = (top & 0x00FF);
}

void Timer::disable(void)
{
    oldPrescaler = (prescaler==0) ? oldPrescaler : prescaler;
    Timer::setPrescaler(0);
}

void Timer::enable(void)
{
    Timer::setPrescaler(oldPrescaler);
}

uint16_t Timer::getPrescaler(void)
{
    return prescaler;
}

uint16_t Timer::getTop(void)
{
    return top;
}

long Timer::getFrequency(void)
{
    if(prescaler == 0)
        return 0;
    else
        return (long)(16000000 / prescaler / (1+top));
}

bool Timer::isActive(void)
{
    return (prescaler!=0);
}

uint8_t Timer::loopInc(void)
{
    if(loopCnt+1 >= loopMax)
        loopCnt=0;
    else
        loopCnt++;

    return loopCnt;
}

uint8_t Timer::getLoopCount(void)
{
    return loopCnt;
}
