/*
 * motor.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "motor.hpp"


Motor::Motor(volatile uint8_t *_dPort, uint8_t _bit1, uint8_t _bit2, volatile uint8_t *_ocr)
{
    dPort = _dPort;
    bit1 = _bit1;
    bit2 = _bit2;
    ocr = _ocr;

    // calculate address of data direction register for direction pins
    volatile uint8_t *dDdr = dPort-1;
    // set pins as output
    *dDdr |= (1<<bit1) | (1<<bit2);
    // turn motor off
    *dPort &= ~((1<<bit1) | (1<<bit2));
}

void Motor::setVelocity(int16_t _velocity)
{
    // set motor direction
    switch((_velocity==0) ? 0 : (_velocity / abs(_velocity)))
    {
    case 0:
        *dPort |= (1<<bit1); // high
        *dPort |= (1<<bit2); // high
        break;
    case +1:
        *dPort |= (1<<bit1); // high
        *dPort &= ~(1<<bit2); // low
        break;
    case -1:
        *dPort &= ~(1<<bit1); // low
        *dPort |= (1<<bit2); // high
        break;
    }

    // calculate dutycycle (remove sign and set upper limit)
    velocity = abs(_velocity) & 0xFF;
    // set new dutycycle
    *ocr = velocity;
    // append removed sign and save
    velocity *= (_velocity==0) ? 0 : (_velocity / abs(velocity));
}

int16_t Motor::getVelocity(void)
{
    return velocity;
}

void Motor::brake(void)
{
    Motor::setVelocity(0);
}
