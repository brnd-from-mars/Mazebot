/*
 * button.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef BUTTON_HPP_
#define BUTTON_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "toggleSwitch.hpp"


class Button : protected ToggleSwitch
{
    public:

        Button(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted);

        void update(void);

        bool getState(void);

    private:

        bool lastState;

        bool pressed;

        uint16_t debouncingCount;

};


#endif
