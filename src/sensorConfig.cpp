/*
 * sensorConfig.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "sensorConfig.hpp"


uint8_t nextSharp;


void sensorInit(void)
{
    // adc
    AnalogDigitalConverter adc = AnalogDigitalConverter();

    // ramp
    ramp = new Ramp(&adc, 0x3);

    // light
    AnalogPin *aLeft = new AnalogPin(&adc, 0xA);
    AnalogPin *aRight = new AnalogPin(&adc, 0x9);
    light = new Light(&PORTA, 2, aLeft, aRight);

    Serial.println("test");

    // sharp
    saFront = new SharpArray<3>(&adc);
    sharp[0] = saFront->setSharp(0, 0xF);
    sharp[1] = saFront->setSharp(1, 0xB);
    sharp[2] = saFront->setSharp(2, 0x0);

    saRight = new SharpArray<2>(&adc);
    sharp[3] = saRight->setSharp(0, 0x1);
    sharp[4] = saRight->setSharp(1, 0x6);

    saBack = new SharpArray<2>(&adc);
    sharp[5] = saBack->setSharp(0, 0x5);
    sharp[6] = saBack->setSharp(1, 0x8);

    saLeft = new SharpArray<2>(&adc);
    sharp[7] = saLeft->setSharp(0, 0x7);
    sharp[8] = saLeft->setSharp(1, 0x4);

    // melexis
    meLeft = new Melexis(0xA0);
    meRight = new Melexis(0xA4);

    // other
    nextSharp = 0;
}

void sensorUpdateMain(void)
{
    meLeft->read();
    meRight->read();
}

void sensorUpdateTimer(void)
{
    sharp[nextSharp]->update();
    ramp->update();
    light->update();

    if(nextSharp+1 >= 9)
        nextSharp = 0;
    else
        nextSharp++;
}