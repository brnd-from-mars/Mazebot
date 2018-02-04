/*
 * eepromCalibratable.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef EEPROMCALIBRATABLE_H_
#define EEPROMCALIBRATABLE_H_


#include <Arduino.h>


template<typename T>
class eepromCalibratable
{
    
    public:

        eepromCalibratable(uint16_t _start, uint8_t _amount);

        void writeValue(uint8_t _num, T _value);

        T readValue(uint8_t _num);

    protected:

        uint16_t start;

        uint8_t amount;

        uint8_t size;

    private:

};


#endif
