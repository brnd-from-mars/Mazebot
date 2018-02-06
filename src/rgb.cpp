/*
 * rgb.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "rgb.hpp"


RGB::RGB(volatile uint8_t *_ocrR, volatile uint8_t *_ocrG, volatile uint8_t *_ocrB)
{
    ocrR = _ocrR;
    ocrG = _ocrG;
    ocrB = _ocrB;
}


void RGB::set(uint8_t _r, uint8_t _g, uint8_t _b)
{
    // if pointer is not NULL
    // (-> pin is used)
    if(ocrR)
        // set new dutycycle
        *ocrR = _r;
    if(ocrG)
        *ocrG = _g;
    if(ocrB)
        *ocrB = _b;
}


void RGB::off(void)
{
    RGB::set(0, 0, 0);
}
