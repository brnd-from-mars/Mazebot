/*
 * eepromCalibratable.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "eepromCalibratable.hpp"


template<typename T>
EepromCalibratable<T>::EepromCalibratable(uint16_t _start, uint8_t _amount)
{
    start = _start;
    amount = _amount;

    // calculate size of each entry
    size = sizeof(T);
}

template<typename T>
void EepromCalibratable<T>::writeValue(uint8_t _num, T _value)
{
    // self-explainary
    if(_num >= amount)
        return;
    
    // calculate address of the first byte for the given index
    uint16_t addr = start + _num*size;

    // reset EEPROM data register
    EEDR = 0;

    // disable interrupts that could disturb writing process
    SREG &= ~(1<<SREG_I);

    // iterate through all bytes of the value
    for(uint8_t i=0; i<size; i++)
    {
        // selct current byte of the value
        uint8_t data = (_value & (0xFF<<(8*i))) >> (8*i);

        // wait during EEPROM is in previous writing process
        while(EECR & (1<<EEPE));
        // get address for current byte inside EEPROM
        EEAR = addr + i;
        // set data register
        EEDR = data;
        // set writing mode
        EECR |= (1<<EEMPE);
        // initiate writing process
        EECR |= (1<<EEPE);
    }

    // enable interrupts
    SREG |= (1<<SREG_I);
}

template<typename T>
T EepromCalibratable<T>::readValue(uint8_t _num)
{
    // self-explainary
    if(_num >= amount)
        return 0;

    // initiate return value
    T value=0;

    // calculate address of the first byte for the given index
    uint16_t addr = start + _num*size;

    // disable interrupts that could disturb writing process
    SREG &= ~(1<<SREG_I);

    // iterate through all bytes of the value    
    for(uint8_t i=0; i<size; i++)
    {
        // wait during EEPROM is in previous writing process
        while(EECR & (1<<EEPE));
        // get address for current byte inside EEPROM
        EEAR = addr + i;
        // initiate reading process
        EECR |= (1<<EERE);
        // copy data as fast as possible to non-volatile storage
        uint8_t data = EEDR;

        // insert data bits into value
        value |= (data<<(8*i));
    }

    // enable interrupts
    SREG |= (1<<SREG_I);

    return value;
}


template class EepromCalibratable<uint16_t>;
