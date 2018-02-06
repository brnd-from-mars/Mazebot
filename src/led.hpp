/*
 * led.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef LED_HPP_
#define LED_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "rgb.hpp"


/**
 * @brief A class that represents a single color led
 * 
 * This class represents a single color led. It varies the dutycycle for
 * the pin to change the led's brightness. Its superclass is
 * RGB. The Led class works just like the RGB class, but it disables
 * two of the outputs of the RGB class by setting there pointer to
 * the output on compare math register to a NULL pointer.
 */
class Led : protected RGB
{
    public:

        /**
         * @brief The constructor for the Led class
         * 
         * This constructor just calls the superclass' constructor
         * by disabling two of its output pins.
         * 
         * @param _ocr dutycycle register for the output pin
         */
        Led(volatile uint8_t *_ocr);

        /**
         * @brief The function that sets new dutycycle
         * 
         * @param _brightness new brightness for the led
         */
        void set(uint8_t _brightness);

};


#endif
