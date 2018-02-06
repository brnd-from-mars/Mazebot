/*
 * button.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "button.hpp"


Button::Button(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted)
    : ToggleSwitch::ToggleSwitch(_pIn, _bit, _inverted)
{
    lastState = false;

    pressed = false;

    debouncingCount = 0;
}

void Button::update(void)
{
    bool state = ToggleSwitch::getState();

    if(debouncingCount != 0)
        debouncingCount--;

    if((!lastState) && state)
    {
        if(debouncingCount==0)
        {
            pressed = true;
            debouncingCount = DEBOUNCING;
        }
    }
    else
        pressed = false;

    lastState = state;
}

bool Button::getState(void)
{
    return pressed;
}