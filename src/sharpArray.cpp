/*
 * sharpArray.cpp
 * created by Brendan Berg on 05.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "sharpArray.hpp"


template<int N>
SharpArray<N>::SharpArray(AnalogDigitalConverter *_adc, uint8_t _sharpChannels[N])
{
    for(uint8_t i=0; i<N; i++)
        sharp[i] = new Sharp(_adc, _sharpChannels[i]);
}

template<int N>
SharpArray<N>::SharpArray(Sharp *_sharp[N])
{
    for(uint8_t i=0; i<N; i++)
        sharp[i] = _sharp[i];
}

template<int N>
Sharp* SharpArray<N>::getSharp(uint8_t i)
{
    if(i>=N)
        return nullptr;
    
    return sharp[i];
}

template<int N>
bool SharpArray<N>::entireWall()
{
    for(uint8_t i=0; i<N; i++)
        if(sharp[i]->getDistance()<WALLLIMIT_RECOG)
            return false;

    return true;
}
