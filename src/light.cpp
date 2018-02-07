/*
 * light.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "light.hpp"


Light::Light(volatile uint8_t *_ledPort, uint8_t _ledBit, AnalogPin *_sensorLeft, AnalogPin *_sensorRight)
{
    ledPort = _ledPort;
    ledBit = _ledBit;
    sensorLeft = _sensorLeft;
    sensorRight = _sensorRight;

    // calculate address of data direction register for led pin
    volatile uint8_t *ddr = ledPort-1;
    // set pin pin as output
    *ddr |= (1<<ledBit);

    loopCnt = 0;
}

void Light::update(void)
{
    // turn on leds
    if(loopCnt == 10)
        Light::prepare();

    // 5ms later: read sensor data
    if(loopCnt == 15)
        Light::read();


    // function cycles throgh 50 loop counts
    if(loopCnt+1 >= 50)
        loopCnt = 0;
    else
        loopCnt++;
}

bool Light::isBlack(void)
{
    // check, if one of the sides detected black
    return ((darknessLeft > DARKNESS_BLACK) || (darknessRight > DARKNESS_BLACK));
}

bool Light::isSilver(void)
{
    // check, if one of the sides detected black
    return ((darknessLeft < DARKNESS_SILVER) || (darknessLeft < DARKNESS_SILVER));
}

void Light::prepare(void)
{
    // turn high power led on
    *ledPort |= (1<<ledBit);
}

void Light::read(void)
{
    // read left sensor
    sensorLeft->read();
    darknessLeft = sensorLeft->getLastValue();

    // read right sensor
    sensorRight->read();
    darknessRight = sensorRight->getLastValue();

    // rutn high power led off
    *ledPort &= ~(1<<ledBit);
}