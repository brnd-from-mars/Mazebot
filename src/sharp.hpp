/*
 * sharp.hpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef SHARP_HPP_
#define SHARP_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "analogPin.hpp"


/**
 * @brief A class that represents a sharp
 * 
 * This class represents a sharp infrared proximity
 * sensor. Because a sharp is a analog sensor, this
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
         * @param _channel the multiplexer channel the sharp is connected to
         */
        Sharp(AnalogDigitalConverter *_adc, uint8_t _channel);

        /**
         * @brief A function that flters and stores the measured values
         */
        void update(void);

        /**
         * @brief A function that returns the smoothed out distance
         * 
         * This function smooths out the sharp values by calculating
         * the average of a certain amount of samples.
         * 
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
