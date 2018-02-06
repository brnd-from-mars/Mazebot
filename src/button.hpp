/*
 * button.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef BUTTON_HPP_
#define BUTTON_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "toggleSwitch.hpp"


/**
 * @brief A class that represents a button
 * 
 * This class represents a button. With its superclass
 * ToggleSwitch it can detect wheter the button is pressed or not.
 * This class detects the start of the pressing of a button and
 * manages to filter out effects of the button bouncing.
 */
class Button : protected ToggleSwitch
{
    public:

        /**
         * @brief The constructor of the Button class
         * 
         * This constructor calls the superclass' constructor and
         * initializes the variables needed for the debouncing and
         * start-press-recognition.
         * 
         * @param _pIn the PIN register containing the input
         * @param _bit the bit of that pin in the register
         * @param _inverted is the potential inverted or not?
         */
        Button(volatile uint8_t *_pIn, uint8_t _bit, bool _inverted);

        /**
         * @brief The function that detects a start of a button press
         * and bouncings
         * 
         * This function should not get called by the timer interrupt.
         * If so, and the timer interrupts the main loop multiple times during one
         * runthrough, the detected press will maybe get discarded.
         */
        void update(void);

        /**
         * @brief The function that returns wheter the button got pressed
         * before the last update call
         * 
         * @return true button press started previously
         * @return false no new butotn press detected
         */
        bool getState(void);

    private:

        /**
         * @brief The true button state at the end of the last update call
         */
        bool lastState;

        /**
         * @brief Was the button pressed immediatly before the last update call?
         */
        bool pressed;

        /**
         * @brief A loop counter for the debouncing logic
         * 
         * This loop counter starts decrementing when a button
         * press is detected. Only after it reaches zero the
         * detection is allowed to start again.
         */
        uint16_t debouncingCount;

};


#endif
