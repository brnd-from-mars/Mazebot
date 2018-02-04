/*
 * sharp.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef SHARP_HPP_
#define SHARP_HPP_


#include <Arduino.h>
#include "config.hpp"
#include "analogDigitalConverter.hpp"
#include "analogPin.hpp"


/**
 * @brief A class that represents a sharp
 * 
 * This class represents a sharp infrared proximity
 * sensor. because a sharp is a analog sensor, this
 * class' superclass is AnalogPin. This class smooths out
 * the distance values.
 */
class Sharp : protected AnalogPin
{
    public:

        /**
         * @brief The constructor of the Sharp class
         * 
         * This function calls the constructor of the superclass
         * and initializes the first samples of measurements.
         * 
         * @param _adc a pointer to a adc object
         * @param _channel the multiplexer channel the pin is connected to
         */
        Sharp(AnalogDigitalConverter *_adc, uint8_t _channel);

        /**
         * @brief A function that stores the measured values
         */
        void read(void);

        /**
         * @brief A function that returns a smoothed out distance
         * 
         * This function filters the sharp values with the minimum
         * value of config->SHARP_FILTER  and calculates the average
         * of the last config->SHARPS_SAMPLES samples.

         * @return uint16_t the enhenced distacne value
         */
        uint16_t getDistance(void);

    private:

        /**
         * @brief The last measured values
         */
        uint16_t lastValues[SHARP_SAMPLES];
        
};


#endif
