/*
 * accelerometer.hpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef ACCELEROMETER_HPP_
#define ACCELEROMETER_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "analogPin.hpp"


/**
 * @brief A class that represents an accelerometer
 * 
 * This class represents an accelerometer. Because the used
 * ADXL335 is a analog sensor, this class' superclass is AnalogPin.
 * This class smooths out the acceleration data.
 */
class Accelerometer : protected AnalogPin
{
    public:

        /**
         * @brief The constructor of the Accelerometer class
         * 
         * This function calls the constructor of the superclass
         * and initializes the first samples of measurements.
         * 
         * @param _adc a pointer to a adc object
         * @param _channel the multiplexer channel the accelerometer is connected to
         */
        Accelerometer(AnalogDigitalConverter *_adc, uint8_t _channel);

        /**
         * @brief A function that stores the measured values
         */
        void read(void);

        /**
         * @brief A function that returns the smoothed out acceleration
         * 
         * This function calculates the average of the last
         * config->ACCEL_SAMPLES samples.
         * 
         * @return uint16_t the enhenced acceleration
         */
        uint16_t getAcceleration(void);

        /**
         * @brief A function that returns wheather the the Accelerometer
         * is tilted or not
         * 
         * This function compares the current acceleration with
         * config->TILT_UP_LIMIT and config->TILT_DOWN_LIMIT.
         *
         * @return -1 accelerometer is tilted downward
         * @return 0 accelerometer is not tilted
         * @return 1 accelerometer is tilted upwards
         */
        int8_t getTiltState(void);

    private:

        /**
         * @brief The last measured values
         */
        uint16_t lastValues[ACCEL_SAMPLES];

};


#endif
