/*
 * led.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "led.hpp"


Led::Led(volatile uint8_t *_ocr)
    : RGB::RGB(_ocr, NULL, NULL)
{
    
}

void Led::set(uint8_t _brightness)
{
    RGB::set(_brightness, 0, 0);
}
