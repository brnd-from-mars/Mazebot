/*
 * analogDigitalConverter.hpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef ANALOGDIGITALCONVERTER_HPP_
#define ANALOGDIGITALCONVERTER_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that controls the analog digital converter
 * 
 * This class controls the build-in ADC. It can set the multiplexer
 * channel and control the sampling.
 */
class AnalogDigitalConverter
{
    public:

        /**
         * @brief The constructor of the AnalogDigitalConverter class
         * 
         * This function enables the ADC and sets the prescaler.
         * For more information please have a look at the official
         * Atmega2560 datasheet (Sections 26).
         */
        AnalogDigitalConverter();

        /**
         * @brief A function that sets the multiplexer channel
         * 
         * @param _channel the channel identifier (e.g. ADC6->6)
         */
        void setChannel(uint8_t _channel);

        /**
         * @brief A function that returns the current channel
         * 
         * @return uint8_t the channel identifier
         */
        uint8_t getChannel(void);

        /**
         * @brief A function that initiates the reading process
         * and disables all interrupts
         */
        void startReading(void);

        /**
         * @brief A function that informs about the current reading progress
         * 
         * @return true sampling has not finished
         * @return false sampling is ready
         */
        bool notReady(void);

        /**
         * @brief A function that reactivates the interrupts
         */
        void finishReading(void);

    private:

        /**
         * @brief The currently selected channel
         */
        uint8_t currentChannel;

};


#endif
