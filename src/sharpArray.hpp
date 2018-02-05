/*
 * sharpArray.hpp
 * created by Brendan Berg on 05.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef SHARPARRAY_HPP_
#define SHARPARRAY_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "sharp.hpp"


template<int N>
class SharpArray
{
    public:

        SharpArray(AnalogDigitalConverter *_adc, uint8_t _sharpChannels[N]);

        SharpArray(Sharp *_sharp[N]);

        Sharp* getSharp(uint8_t i);

        bool entireWall();

    private:

        Sharp *sharp[N];
};

#endif
