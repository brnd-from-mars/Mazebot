/*
 * motor.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "motor.hpp"


motor::motor(volatile uint8_t *_dPort, uint8_t _bit1, uint8_t _bit2, pwm *_pinP)
{
    dPort = _dPort;
    bit1 = _bit1;
    bit2 = _bit2;
    pinP = _pinP;

    // calculate address of data direction register for direction pins
    volatile uint8_t *dDdr = dPort-1;
    // set pins as output
    *dDdr |= (1<<bit1) | (1<<bit2);
    // turn motor off
    *dPort &= ~((1<<bit1) | (1<<bit2));
}

void motor::setVelocity(int16_t _velocity)
{
    // set motor direction
    switch((_velocity==0) ? 0 : (_velocity / abs(_velocity)))
    {
    case 0:
        *dPort |= (1<<bit1);
        *dPort |= (1<<bit2);
        break;
    case +1:
        *dPort |= (1<<bit1);
        *dPort &= ~(1<<bit2);
        break;
    case -1:
        *dPort &= ~(1<<bit1);
        *dPort |= (1<<bit2);
        break;
    }

    // calculate dutycycle (remove sign and set upper limit)
    velocity = ((255>abs(_velocity)) ? abs(_velocity) : 255);
    // set new dutycycle
    (*pinP).set(velocity);
    // append removed sign and save
    velocity *= (_velocity==0) ? 0 : (_velocity / abs(velocity));
}

int16_t motor::getVelocity(void)
{
    return velocity;
}

void motor::brake(void)
{
    motor::setVelocity(0);
}
