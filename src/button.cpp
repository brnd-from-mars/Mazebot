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
    // get true state of button
    bool state = ToggleSwitch::getState();

    pressed = false;

    if(debouncingCount != 0)
        debouncingCount--;
    else if((!lastState) && state)
    {
        // if last press-start is a while ago
        // and new press started
        pressed = true;
        debouncingCount = DEBOUNCING;
    }

    lastState = state;
}

bool Button::getState(void)
{
    return pressed;
}