/*
 * timer.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef TIMER_HPP_
#define TIMER_HPP_


#include <Arduino.h>


class Timer
{

    public:

        Timer(uint16_t _prescaler, uint16_t _top);

        void setPrescaler(uint16_t _prescaler);

        void setTop(uint16_t _top);

        void disable(void);

        void enable(void);

        uint16_t getPrescaler(void);

        uint16_t getTop(void);

        long getFrequency(void);

        bool isActive(void);

    private:

        uint16_t prescaler;

        uint16_t oldPrescaler;

        uint16_t top;

};


#endif
