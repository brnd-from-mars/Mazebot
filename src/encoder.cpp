/*
 * encoder.cpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "encoder.hpp"


Encoder::Encoder(volatile uint8_t *_pInStep, uint8_t _bitStep, volatile uint8_t *_pInDir, uint8_t _bitDir)
{
    pInStep = _pInStep;
    pInDir = _pInDir;

    bitStep = _bitStep;
    bitDir = _bitDir;

    // determine starting potential of the step pin
    lastPot = *pInStep & (1<<bitStep);

    // initialize steps
    steps = 0;
}

void Encoder::update(void)
{
    // determine current step pin potential
    bool pot = *pInStep & (1<<bitStep);
    // if step potential has changes
    if(pot != lastPot)
    {
        // change last Potential
        lastPot = pot;
        if(lastPot)
        {
            // update steps in according to the direction pin
            bool dir = *pInDir & (1<<bitDir);
            // (dir<<1)-1 = 2*dir - 1
            // dir:     1   0
            // Delta:   1   -1
            steps += (dir<<1)-1;
        }
    }
}

long Encoder::getSteps(void)
{
    return steps;
}

void Encoder::reset(void)
{
    steps = 0;
}
