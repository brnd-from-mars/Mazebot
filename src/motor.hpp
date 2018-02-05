/*
 * motor.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef MOTOR_HPP_
#define MOTOR_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "encoder.hpp"


/**
 * @brief A class that represents a motor
 * 
 * This class represents a dc motor. It can control direction and velocity.
 * Its superclass is Encoder, so a motor object can read its own covered steps.
 */
class Motor : public Encoder
{
    public:

        /**
         * @brief The constructor of the Motor class
         * 
         * This function sets the motor pins to output stores the pointer to the
         * output compare register for the velocity control and calls the superclass'
         * constrctor.
         * 
         * @param _dPort a pointer to the PORT of both direction pins
         * @param _bit1 the bit of the first pin in _dPort
         * @param _bit2 the bit of the second pin in _dPort
         * @param _ocr a pointer to the register containing the current compare match value (i.e. dutycycle)
         * @param _pInStep a pointer to the PIN register containing the step pin
         * @param _bitStep the bit of the previously mentioned pin in the register
         * @param _pInDir a pointer to the PIN register containing the direction pin
         * @param _bitDir the bit of the previously mentioned pin in the register
         */
        Motor(volatile uint8_t *_dPort, uint8_t _bit1, uint8_t _bit2, volatile uint8_t *_ocr, volatile uint8_t *_pInStep, uint8_t _bitStep, volatile uint8_t *_pInDir, uint8_t _bitDir);

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
         */
        volatile uint8_t *dPort;

        /**
         * @brief The bit of the first pin in dPort
         */
        uint8_t bit1;

        /**
         * @brief The bit of the second pin in dPort
         */
        uint8_t bit2;

        /**
         * @brief A pointer to the register containing the current compare match value (i.e. dutycycle)
         */
        volatile uint8_t *ocr;
        
        /**
         * @brief The current velocity between -255 (backwards) and 255 (forwards)
         */
        int16_t velocity;

};


#endif
