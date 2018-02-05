/*
 * timer.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "timer.hpp"


Timer::Timer(uint16_t _prescaler, uint16_t _top, uint16_t _loopMax)
{
    // reset timer/counter control registers
    TCCR5A = 0;
    TCCR5B = 0;
    TCCR5C = 0;
    // reset timer interrupt mask
    TIMSK5 = 0;

    // set timer 1 to clear timer on compare mode
    TCCR5B |= (1<<WGM52);

    // activate interrupt for compare A
    TIMSK5 |= (1<<ICIE5) | (1<<OCIE5A);

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
    TCCR5B &= 0xF8;
    
    switch(_prescaler)
    {
    case 1:
        TCCR5B |= (1<<CS50);
        break;
    case 8:
        TCCR5B |= (1<<CS51);
        break;
    case 64:
        TCCR5B |= (1<<CS51) | (1<<CS50);
        break;
    case 256:
        TCCR5B |= (1<<CS52);
        break;
    case 1024:
        TCCR5B |= (1<<CS52) | (1<<CS50);
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
    OCR5AH = (top & 0xFF00) >> 8;
    OCR5AL = (top & 0x00FF);
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

uint16_t Timer::loopInc(void)
{
    if(loopCnt+1 >= loopMax)
        loopCnt=0;
    else
        loopCnt++;

    return loopCnt;
}

uint16_t Timer::getLoopCount(void)
{
    return loopCnt;
}
