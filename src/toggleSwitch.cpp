/*
 * toggleSwitch.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "toggleSwitch.hpp"


ToggleSwitch::ToggleSwitch(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted)
{
    pIn = _pIn;
    bit = _bit;
    inverted = _inverted;
}

bool ToggleSwitch::getState(void)
{
    // read inout state and invert if necessary
    return ((bool)(*pIn & (1<<bit))) != inverted;
}
