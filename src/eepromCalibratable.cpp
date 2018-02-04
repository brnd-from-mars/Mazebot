/*
 * eepromCalibratable.cpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "eepromCalibratable.hpp"


template <typename T>
EepromCalibratable<T>::EepromCalibratable(uint16_t _start, uint8_t _amount)
{
    start = _start;
    amount = _amount;

    size = sizeof(T);
}

template <typename T>
void EepromCalibratable<T>::writeValue(uint8_t _num, T _value)
{
    if(_num >= amount)
        return;
    
    uint16_t addr = start + _num*size;

    EEDR = 0;

    SREG &= ~(1<<SREG_I);
    for(uint8_t i=0; i<size; i++)
    {
        uint8_t data = (_value & (0xFF<<(8*i))) >> (8*i);

        while(EECR & (1<<EEPE));
        EEAR = addr + i;
        EEDR = data;
        EECR |= (1<<EEMPE);
        EECR |= (1<<EEPE);
    }
    SREG |= (1<<SREG_I);
}

template <typename T>
T EepromCalibratable<T>::readValue(uint8_t _num)
{
    if(_num >= amount)
        return 0;

    T value=0;

    uint16_t addr = start + _num*size;

    SREG &= ~(1<<SREG_I);
    for(uint8_t i=0; i<size; i++)
    {
        while(EECR & (1<<EEPE));
        EEAR = addr + i;
        EECR |= (1<<EERE);
        uint8_t data = EEDR;

        value |= (data<<(8*i));
    }
    SREG |= (1<<SREG_I);

    return value;
}