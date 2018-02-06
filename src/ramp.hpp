/*
 * ramp.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef RAMP_HPP_
#define RAMP_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "accelerometer.hpp"


/**
 * @brief A class that recognizes ramps
 * 
 * Because this class' superclass is Accelerometer, Ramp is
 * able to detect wheter the robot is tilted upwards, downwards
 * or neihter of them. With that information in hand, it can
 * determine very confidently if the tilt is a result of a 
 * ramp (that should get detected) or a speed bumper.
 */
class Ramp : protected Accelerometer
{
    public:

        /**
         * @brief The constructor of the Ramp class
         * 
         * This function calls the superclass' constructor
         * and initializes the variables necessary for the
         * ramp detection.
         * 
         * @param _adc a pointer to a adc object
         * @param _channel the multiplexer channel the accelerometer is connected to
         */
        Ramp(AnalogDigitalConverter *_adc, uint8_t _channel);

        /**
         * @brief The function that detects over time if the robot is on a ramp
         * 
         * The ramp detection uses the fact, that the robot is tilted for a longer
         * timer on the ramp as on a speed bumper.
         * 
         * This recognition depends on the timing and how often this function is called,
         * so use it rather inside the timing loop.
         */
        void update(void);

        /**
         * @brief The function that returns the current ramp state
         * 
         * @return 1 on ramp upwards
         * @return 0 not on ramp
         * @return -1 on ramp downwars
         */
        int8_t getState(void);
    
    private:

        /**
         * @brief The current ramp state
         * 
         * 1: on ramp upwards
         * 0: not on ramp
         * -1: on ramp downwars
         */
        int8_t rampState;

        /**
         * @brief a confidence score for driving on a ramp upwards
         */
        uint8_t maybeOnRampUp;

        /**
         * @brief A confidence score for driving on a ramp downwards
         */
        uint8_t maybeOnRampDown;

};


#endif
