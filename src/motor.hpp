/*
 * motor.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef MOTOR_HPP_
#define MOTOR_HPP_


#include <Arduino.h>


/**
 * @brief A class representing a motor
 * 
 * This class represents a dc motor. It can control direction and velocity.
 */
class motor
{
    public:

        /**
         * @brief The constructor of the motor class
         * 
         * This function sets the motor pins to output and starts a suitable pwm timer.
         * For more information please have a look at the official Atmega2560 datasheet (Sections 16 and 17).
         * 
         * @param _dPort a pointer to the PORT of both direction pins
         * @param _bit1 the bit of the first pin in _dPort
         * @param _bit2 the bit of the second pin in _dPort
         * @param _vPort a pointer to the PORT of the pwm pin
         * @param _bitv the bit of the pin in _vPort
         * @param _timer the atmega timer (0/1/3/4/5) controlling the pwm pin
         * @param _comparator the comparator (A->1, B->2, C->3) connected to the pwm pin
         * @param _dutycycle a pointer to the register containing the curretn compare match value (i.e. dutycycle)
         */
        motor(volatile uint8_t *_dPort, uint8_t _bit1, uint8_t _bit2, volatile uint8_t *_vPort, uint8_t _bitv, uint8_t _timer, uint8_t _comparator, volatile uint8_t *_dutycycle);

        /**
         * @brief A function that sets the velocity
         * 
         * @param _velocity the velocity between -255 (backwards) and 255 (forwards)
         */
        void setVelocity(int16_t _velocity);

        /**
         * @brief A function that returns the current velocity
         * 
         * @return int16_t the velocity between -255 (backwards) and 255 (forwards)
         */
        int16_t getVelocity(void);

        /**
         * @brief A convenient function to set the velocity to 0 
         */
        void brake(void);

    private:

        /**
         * @brief A pointer to the PORT of both direction pins
         * 
         */
        volatile uint8_t *dPort;

        /**
         * @brief The bit of the first pin in dPort
         * 
         */
        uint8_t bit1;

        /**
         * @brief The bit of the second pin in dPort
         * 
         */
        uint8_t bit2;

        /**
         * @brief A pointer to the register containing the curretn compare match value (i.e. dutycycle)
         * 
         */
        volatile uint8_t *dutycycle;
        
        /**
         * @brief The current velocity between -255 (backwards) and 255 (forwards)
         * 
         */
        int16_t velocity;
};


#endif
