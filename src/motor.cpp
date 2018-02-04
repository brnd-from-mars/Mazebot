/*
 * motor.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "motor.hpp"


motor::motor(volatile uint8_t *_dPort, uint8_t _bit1, uint8_t _bit2, volatile uint8_t *_vPort, uint8_t _bitv, uint8_t _timer, uint8_t _comparator, volatile uint8_t *_dutycycle)
{
    dutycycle = _dutycycle;
    dPort = _dPort;
    bit1 = _bit1;
    bit2 = _bit2;

    // calculate address of data direction register for direction pins
    volatile uint8_t *dDdr = dPort-1;
    // set pins as output
    *dDdr |= (1<<bit1) | (1<<bit2);
    // turn motor off
    *dPort &= ~((1<<bit1) | (1<<bit2));

    // calculate address of data direction register for pwm pin
    volatile uint8_t *vDdr = _vPort-1;
    // set pin as output
    *vDdr |= (1<<_bitv);

    switch(_timer)
    {
    // 8 bit timer
    case 0:
        // reset count control registers
        TCCR0A &= 0xF0;
        TCCR0B &= 0x00;
        // choose output compare pin and select non-inverted pwm
        TCCR0A |= (1<<(9-2*_comparator));
        // set mode to fast pwm
        TCCR0A |= (1<<WGM01) | (1<<WGM00);
        // set prescaler -> 62.5kHz
        TCCR0B |= (1<<CS00);
        break;
    // 16 bit timer
    case 3:
        // reset count control registers
        TCCR3A &= 0xFC;
        TCCR3B &= 0x00;
        // choose output compare pin and select non-inverted pwm
        TCCR3A |= (1<<(9-2*_comparator));
        // set mode to fast pwm
        TCCR3A |= (1<<WGM30);
        TCCR3B |= (1<<WGM32);
        // set prescaler -> 62.5kHz
        TCCR3B |= (1<<CS30);
        break;
    case 4:
        TCCR4A &= 0xFC;
        TCCR4B &= 0x00;
        TCCR4A |= (1<<(9-2*_comparator)) | (1<<WGM40);
        TCCR4B |= (1<<WGM42) | (1<<CS40);
        break;
    case 5:
        TCCR5A &= 0xFC;
        TCCR5B &= 0x00;
        TCCR5A |= (1<<(9-2*_comparator)) | (1<<WGM50);
        TCCR5B |= (1<<WGM52) | (1<<CS50);
        break;
    }
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
    *dutycycle = velocity;
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
