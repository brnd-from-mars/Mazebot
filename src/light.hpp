/*
 * light.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef LIGHT_HPP_
#define LIGHT_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "analogPin.hpp"


/**
 * @brief A class that represents two light sensors and high power leds
 * 
 * This class represents two light sensors and a highpower led. It is used to
 * detect a black and silver floor at the bottom. The sensor read out will get
 * carry out in two steps:
 * 
 * Step 1: activating the high power leds to light up the floor
 * 
 * Step 2: (around 5ms later) reading out both light sensors and disabling the leds
 * 
 * The timing is controlled by the update function with the assumption that it is called
 * at a 1kHz rate.
 * 
 * If the leds would stay constantly on they would reach a critical temperature.
 * 
 */
class Light
{
    public:

        /**
         * @brief The constructor of the Light class
         * 
         * This constructor sets the pin connected to the high power leds to output and
         * stores the parameters.
         * 
         * @param _ledPort a pointer to the PORT of the high power leds
         * @param _ledBit the bit of the pin connected to the leds in _ledPort
         * @param _sensorLeft the analog pin connected to the left light sensor
         * @param _sensorRight the analog pin connected to the right light sensor
         */
        Light(volatile uint8_t *_ledPort, uint8_t _ledBit, AnalogPin *_sensorLeft, AnalogPin *_sensorRight);

        /**
         * @brief The function that controls the timing of the measurement
         * 
         * This function assuemes that it is called at a 1kHz rate
         */
        void update(void);

        /**
         * @brief The function that returns wether the last measurement detected black or not
         * 
         * @return true tile is black
         * @return false tile is not black
         */
        bool isBlack(void);

        /**
         * @brief The function that returns wether the last measurement detected silver or not
         * 
         * @return true tile is silver
         * @return false tile is not silver
         */
        bool isSilver(void);

    private:

        /**
         * @brief The function that prepares the leds for measurement
         * 
         * This function turns on the high power leds
         */
        void prepare(void);

        /**
         * @brief The function that performs the measurement
         * 
         * This function reads the analog data and turns off the leds
         */
        void read(void);

        /**
         * @brief a pointer to the PORT of the high power led
         */
        volatile uint8_t *ledPort;

        /**
         * @brief the bit of the pin connected to the leds in ledPort
         * 
         */
        uint8_t ledBit;

        /**
         * @brief the analog pin connected to the left light sensor
         */
        AnalogPin *sensorLeft;

        /**
         * @brief the analog pin connected to the right light sensor
         */
        AnalogPin *sensorRight;

        /**
         * @brief a loop counter to handle measurement timing
         */
        uint8_t loopCnt;

        /**
         * @brief last measured darkness on the LHS
         */
        uint16_t darknessLeft;

        /**
         * @brief last measured darkness on the right hand side
         */
        uint16_t darknessRight;
        
};


#endif
