/*
 * pwm.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef PWM_H_
#define PWM_H_


#include <Arduino.h>


/**
 * @brief A class that represents a pwm pin
 * 
 * This class represents a pwm pin. It sets the pin as output and
 * varies the dutycycle
 */
class pwm
{
    public:

        /**
         * @brief The constructor of the pwm class
         * 
         * This function sets the pwm pin to output and starts a suitable pwm timer.
         * For more information please have a look at the official Atmega2560 datasheet (Sections 16 and 17).
         * 
         * @param _port a pointer to the PORT of the pwm pin
         * @param _bit the bit of the pin in _port
         * @param _timer the atmega timer (0/1/3/4/5) controlling the pwm pin
         * @param _comparator the comparator (A->1, B->2, C->3) connected to the pwm pin
         * @param _dutycycle a pointer to the register containing the curretn compare match value (i.e. dutycycle)
         */
        pwm(volatile uint8_t *_port, uint8_t _bit, uint8_t _timer, uint8_t _comparator, volatile uint8_t *_dutycycle);

        /**
         * @brief A function that changes the dutycycle
         * 
         * @param value the dutycycle from 0 (0%) to 255 (100%)
         */
        void set(uint8_t value);

        /**
         * @brief A function that returns the curretn dutycycle
         * 
         * @return uint8_t the dutycycle from 0 (0%) to 255 (100%) 
         */
        uint8_t get(void);

    private:

        /**
         * @brief a pointer to the register containing the curretn compare match value (i.e. dutycycle)
         */
        volatile uint8_t *dutycycle;

};


#endif
