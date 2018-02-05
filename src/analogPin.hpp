/*
 * analogPin.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef ANALOGPIN_HPP_
#define ANALOGPIN_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "analogDigitalConverter.hpp"


/**
 * @brief A class that represents an analog pin
 * 
 * This class represents an analog pin. It initiates the ADC to
 * start the sampling process and stores its last value. This class is
 * designed for inheritance (e.g. sensor devices).
 */
class AnalogPin
{
    public:

        /**
         * @brief The constructor of the AnalogPin class
         * 
         * This function stores the pin data and starts
         * the first measurement.
         * 
         * @param _adc a pointer to a adc object
         * @param _channel the multiplexer channel the pin is connected to
         */
        AnalogPin(AnalogDigitalConverter *_adc, uint8_t _channel);

        /**
         * @brief A function that starts a reading process
         */
        void read(void);

        /**
         * @brief A function that returns the last measurement
         * 
         * @return uint16_t the last measurement
         */
        uint16_t getLastValue();

    protected:

        /**
         * @brief The last measurement
         */
        uint16_t lastValue;

    private:

        /**
         * @brief A pointer to a adc object
         */
        AnalogDigitalConverter *adc;

        /**
         * @brief The multiplexer channel the pin is connected to
         */
        uint8_t channel;

};


#endif
