/*
 * pwm.hpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef PWM_HPP_
#define PWM_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that creates a pwm pin
 * 
 * This class creates a pwm signal at a given pin. It sets the pin as output and
 * can give a pointer to the register with its dutycycle.
 */
class Pwm
{
    public:

        /**
         * @brief The constructor of the Pwm class
         * 
         * This function sets the pwm pin to output and starts a suitable pwm timer.
         * For more information please have a look at the official Atmega2560 datasheet (Sections 16 and 17).
         * 
         * @param _port a pointer to the PORT of the pwm pin
         * @param _bit the bit of the pin in _port
         * @param _timer the atmega timer (0/1/3/4/5) controlling the pwm pin
         * @param _comparator the comparator (A->1, B->2, C->3) connected to the pwm pin
         */
        Pwm(volatile uint8_t *_port, uint8_t _bit, uint8_t _timer, uint8_t _comparator);

        /**
         * @brief The function that returns a pointer to the register
         * containing  the current compare match value (i.e. dutycycle)
         * 
         * @return uint8_t* a pointer to a register
         */
        volatile uint8_t* getDutycycleRegister();

    private:

        /**
         * @brief A pointer to the register containing the current compare match value (i.e. dutycycle)
         */
        volatile uint8_t *dutycycle;

};


#endif
