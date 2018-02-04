/*
 * eepromCalibratable.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef EEPROMCALIBRATABLE_H_
#define EEPROMCALIBRATABLE_H_


#include <Arduino.h>


/**
 * @brief A class that manages calibration values stored in the EEPROM
 * 
 * This class-template provides a easy way of storing calibration values of
 * different types inside the EEPROM. This class is just for inheritance.
 * 
 * @tparam T the type of single values that get stored inside the EEPROM
 */
template<typename T>
class EepromCalibratable
{
    
    public:

        /**
         * @brief The constructor of the EepromCalibratable class
         * 
         * This function stores start and amount of calibration data and
         * calculates the needed space for a single entry.
         * 
         * @param _start the address of the first byte for that context
         * @param _amount the amount of entries
         */
        EepromCalibratable(uint16_t _start, uint8_t _amount);

        /**
         * @brief The function that stores/updates a new entry inside the EEPROM.
         * 
         * For more information have a look at the official Atmega2560 datasheet
         * (Section 8 and 9).
         * 
         * @param _num index of the entry for the current context
         * @param _value the new value
         */
        void writeValue(uint8_t _num, T _value);

        /**
         * @brief The function that reads a specific entry inside the EEPROM
         * 
         * For more information have a look at the official Atmega2560 datasheet
         * (Section 8 and 9).
         * 
         * @param _num index of the entry for the current context
         * @return T the stored value
         */
        T readValue(uint8_t _num);

    private:

        /**
         * @brief the address of the first byte for that context
         */
        uint16_t start;

        /**
         * @brief the amount of entries
         */
        uint8_t amount;

        /**
         * @brief the size of each entry
         */
        uint8_t size;

};


#endif
