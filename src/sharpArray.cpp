/*
 * sharpArray.cpp
 * created by Brendan Berg on 05.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "sharpArray.hpp"


template<int N>
SharpArray<N>::SharpArray(AnalogDigitalConverter *_adc)
{
    adc = _adc;
}


template<int N>
Sharp* SharpArray<N>::setSharp(uint8_t i, uint8_t _channel)
{
    sharp[i] = new Sharp(adc, _channel);

    return sharp[i];
}

template<int N>
Sharp* SharpArray<N>::getSharp(uint8_t i)
{
    if(i>=N)
        return NULL;
    
    return sharp[i];
}

template<int N>
bool SharpArray<N>::entireWall(void)
{
    // check for all N sharps
    for(uint8_t i=0; i<N; i++)
        // if wall/obstacle detected
        if(sharp[i]->getDistance()<WALLLIMIT_RECOG)
            return false;

    return true;
}

template<int N>
int16_t SharpArray<N>::average(void)
{
    uint16_t sum=0;

    // check for all N sharps
    for(uint8_t i=0; i<N; i++)
    {
        // if no wall/obstacle detected
        if(sharp[i]->getDistance()<WALLLIMIT_ALIGN)
            return 0xFFF;
        // otherwise calculate sum
        sum+=sharp[i]->getDistance();
    }
    
    // and calculate average
    return (int16_t)(sum / N);
}

template<int N>
int16_t SharpArray<N>::misalignment(void)
{
    // check for all N sharps
    for(uint8_t i=0; i<N; i++)
        // if no wall/obstacle detected
        if(sharp[i]->getDistance()<WALLLIMIT_ALIGN)
            return 0xFFF;
    
    // otherwise return difference between first and last sharp
    return ((int16_t)(sharp[0]->getDistance())-(int16_t)(sharp[N-1]->getDistance()));
}


template class SharpArray<2>;
template class SharpArray<3>;
