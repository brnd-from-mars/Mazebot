/*
 * encoder.hpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef ENCODER_HPP_
#define ENCODER_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that represents an encoder
 * 
 * This class represents an encoder. It's update function
 * should get called more than twice as often as the motor is
 * able to perform an encoder step to take every step into
 * account
 */
class Encoder
{
    public:

        /**
         * @brief The constructor of the Encoder class
         * 
         * This constructor stores the given parameters and measures the first
         * time the potential on the step pin.
         * 
         * @param _pInStep a pointer to the PIN register containing the step pin
         * @param _bitStep the bit of the previously mentioned pin in the register
         * @param _pInDir a pointer to the PIN register containing the direction pin
         * @param _bitDir the bit of the previously mentioned pin in the register
         */
        Encoder(volatile uint8_t *_pInStep, uint8_t _bitStep, volatile uint8_t *_pInDir, uint8_t _bitDir);

        /**
         * @brief A fucntion that updates the encoder step counter
         * 
         * This function checks wheater the potential on the step pin changed.
         * If so, and it encountered an rising edge, it uses the direction
         * potential to determine the direction and to update the step counter
         * accordingly.
         */
        void update(void);

        /**
         * @brief A function that returns the current step count
         * 
         * @return long steps since last reset
         */
        long getSteps(void);

        /**
         * @brief A function that resets the step count
         */
        void reset(void);

    private:

        /**
         * @brief A pointer to the PIN register containing the step pin
         */
        volatile uint8_t *pInStep;

        /**
         * @brief The bit of the step pin in the register
         */
        uint8_t bitStep;

        /**
         * @brief A pointer to the PIN register containing the direction pin
         */
        volatile uint8_t *pInDir;

        /**
         * @brief The bit of the direction pin in the register
         */
        uint8_t bitDir;

        /**
         * @brief The last measured potential on the step pin
         */
        bool lastPot;

        /**
         * @brief Steps detected since last reset
         */
        long steps;

};


#endif
