/*
 * timer.hpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef TIMER_HPP_
#define TIMER_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that controls the main timer
 * 
 * This class sets timer 1 to clear timer on compare mode and provides a fast
 * and easy way for changing prescaler and TOP value.
 */
class Timer
{
    public:

        /**
         * @brief The constructor of the Timer class
         * 
         * This function sets the timer 1 to clear timer on compare mode and sets its
         * initial frequency. For more information have a look at the official 
         * Atmega2560 datasheet (Section 17).
         * 
         * @param _prescaler the prescaler (0/1/8/64/256/1024) for the clock
         * @param _top the top value for the counter
         */
        Timer(uint16_t _prescaler, uint16_t _top);

        /**
         * @brief The function to set a new prescaler
         * 
         * @param _prescaler the new prescaler
         */
        void setPrescaler(uint16_t _prescaler);

        /**
         * @brief The function to set a new TOP value
         * 
         * @param _top the new top value
         */
        void setTop(uint16_t _top);

        /**
         * @brief The function to disable the timer
         * 
         * This function works by setting the prescaler to 0.
         */
        void disable(void);


        /**
         * @brief The function to enable the timer
         * 
         * This function works by setting the prescaler to the value
         * before calling the disable function
         */
        void enable(void);

        /**
         * @brief The function that returns the current prescaler
         * 
         * @return uint16_t the current prescaler
         */
        uint16_t getPrescaler(void);

        /**
         * @brief The function that returns the current TOP value
         * 
         * @return uint16_t the current TOP value
         */
        uint16_t getTop(void);

        /**
         * @brief The function that calculates the frequency of the interrupt
         * 
         * The frequency depends on prescaler and the TOP value. The interrupt
         * handler will not get provided by this class and has to be defined
         * outside in the global space.
         * 
         * @return long the current frequency
         */
        long getFrequency(void);

        /**
         * @brief The function that detects wheter the timer is currently active
         * 
         * @return true timer is active
         * @return false timer isn't active
         */
        bool isActive(void);

    private:

        /**
         * @brief The current prescaler
         */
        uint16_t prescaler;

        /**
         * @brief The prescaler at a time before the disable function got called
         */
        uint16_t oldPrescaler;

        /**
         * @brief The current TOP value
         */
        uint16_t top;

};


#endif
