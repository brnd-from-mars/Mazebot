/*
 * pwm.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "pwm.hpp"


Pwm::Pwm(volatile uint8_t *_port, uint8_t _bit, uint8_t _timer, uint8_t _comparator)
{
    // calculate address of data direction register for pwm pin
    volatile uint8_t *ddr = _port-1;
    // set pin as output
    *ddr |= (1<<_bit);

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
        // set pointer to OCR
        dutycycle = &OCR0A+(_comparator-1);
        break;
    case 2:
        TCCR2A &= 0xF0;
        TCCR2B &= 0x00;
        TCCR2A |= (1<<(9-2*_comparator)) | (1<<WGM21) | (1<<WGM20);
        TCCR2B |= (1<<CS20);
        dutycycle = &OCR2A+(_comparator-1);
        break;
    // 16 bit timer
    case 1:
        // reset count control registers
        TCCR1A &= 0xFC;
        TCCR1B &= 0x00;
        // choose output compare pin and select non-inverted pwm
        TCCR1A |= (1<<(9-2*_comparator));
        // set mode to fast pwm
        TCCR1A |= (1<<WGM10);
        TCCR1B |= (1<<WGM12);
        // set prescaler -> 62.5kHz
        TCCR1B |= (1<<CS10);
        // set pointer to OCR
        dutycycle = &OCR1AL+2*(_comparator-1);
        break;
    case 3:
        TCCR3A &= 0xFC;
        TCCR3B &= 0x00;
        TCCR3A |= (1<<(9-2*_comparator)) | (1<<WGM30);
        TCCR3B |= (1<<WGM32) | (1<<CS30);
        dutycycle = &OCR3AL+2*(_comparator-1);
        break;
    case 4:
        TCCR4A &= 0xFC;
        TCCR4B &= 0x00;
        TCCR4A |= (1<<(9-2*_comparator)) | (1<<WGM40);
        TCCR4B |= (1<<WGM42) | (1<<CS40);
        dutycycle = &OCR4AL+2*(_comparator-1);
        break;
    case 5:
        TCCR5A &= 0xFC;
        TCCR5B &= 0x00;
        TCCR5A |= (1<<(9-2*_comparator)) | (1<<WGM50);
        TCCR5B |= (1<<WGM52) | (1<<CS50);
        dutycycle = &OCR5AL+2*(_comparator-1);
        break;
    }
}

volatile uint8_t* Pwm::getDutycycleRegister()
{
    return dutycycle;
}
