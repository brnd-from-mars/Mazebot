/*
 * sharpArray.cpp
 * created by Brendan Berg on 05.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "sharpArray.hpp"


template<int N>
SharpArray<N>::SharpArray(AnalogDigitalConverter *_adc, uint8_t _sharpChannels[N])
{
    // create N new Sharps
    for(uint8_t i=0; i<N; i++)
        sharp[i] = new Sharp(_adc, _sharpChannels[i]);
}

template<int N>
SharpArray<N>::SharpArray(Sharp *_sharp[N])
{
    // store pointers to all N Sharps
    for(uint8_t i=0; i<N; i++)
        sharp[i] = _sharp[i];
}

template<int N>
Sharp* SharpArray<N>::getSharp(uint8_t i)
{
    // return pointer to specific Sharp
    if(i>=N)
        return nullptr;
    
    return sharp[i];
}

template<int N>
bool SharpArray<N>::entireWall()
{
    // check for all N sharps
    for(uint8_t i=0; i<N; i++)
        // if wall/obstacle detected
        if(sharp[i]->getDistance()<WALLLIMIT_RECOG)
            return false;

    return true;
}
