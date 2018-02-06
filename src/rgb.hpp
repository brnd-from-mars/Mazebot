/*
 * rgb.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef RGB_HPP_
#define RGB_HPP_


#include <Arduino.h>
#include "config.hpp"


/**
 * @brief A class that represents a rgb led.
 *
 * This class represents a rgb led. It varies the dutycycles for
 * the red/green/blue pin to changes there intensities. 
 */
class RGB
{
    public:

        /**
         * @brief The constructor of the RGB class
         * 
         * This constructor stores pointers to the registers
         * containing the dutycycles for the pwm pins
         * 
         * @param _ocrR dutycycle register for red constrolling pin
         * @param _ocrG dutycycle register for green constrolling pin
         * @param _ocrB dutycycle register for blue constrolling pin
         */
        RGB(volatile uint8_t *_ocrR, volatile uint8_t *_ocrG, volatile uint8_t *_ocrB);

        /**
         * @brief The function that sets new dutycyles
         * 
         * @param _r new intensity for color red 
         * @param _g new intensity for color green
         * @param _b new intensity for color blue
         */
        void set(uint8_t _r, uint8_t _g, uint8_t _b);

        /**
         * @brief Convinience function to turn led off
         */
        void off(void);

    private:

        /**
         * @brief Dutycycle register for red constrolling pin
         */
        volatile uint8_t *ocrR;

        /**
         * @brief Dutycycle register for green constrolling pin
         */
        volatile uint8_t *ocrG;

        /**
         * @brief Dutycycle register for blue constrolling pin
         */
        volatile uint8_t *ocrB;

};


#endif
