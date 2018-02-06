/*
 * toggleSwitch.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef TOGGLESWITCH_HPP_
#define TOGGLESWITCH_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that represents a toggle switch
 * 
 * This class represents a toggle switch. An object of this
 * class is able to detect wheater the represented switch is
 * turned on or off.
 */
class ToggleSwitch
{
    public:

        /**
         * @brief The contructor of the ToggleSwitch class
         * 
         * This constructor stores the register containing the 
         * input data for the pin and its bit inside this register.
         * 
         * @param _pIn the PIN register containing the input
         * @param _bit the bit of that pin in the register
         * @param _inverted is the potential inverted or not?
         */
        ToggleSwitch(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted);

        /**
         * @brief The function that returns the state of the switch
         * 
         * @return true the switch is activates
         * @return false the switch is deactivated
         */
        bool getState(void);

    private:

        /**
         * @brief The PIN register containing the input
         */
        volatile uint8_t *pIn;

        /**
         * @brief The bit of that pin in the register
         */
        uint8_t bit;

        /**
         * @brief Is the potential inverted or not?
         */
        bool inverted;

};


#endif
