/*
 * toggleSwitch.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef TOGGLESWITCH_HPP_
#define TOGGLESWITCH_HPP_


#include <Arduino.h>
#include "config.hpp"


class ToggleSwitch
{
    public:

        ToggleSwitch(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted);

        bool getState(void);

    private:

        volatile uint8_t *pIn;

        uint8_t bit;

        bool inverted;

};


#endif
