/*
 * sharpArray.hpp
 * created by Brendan Berg on 05.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#ifndef SHARPARRAY_HPP_
#define SHARPARRAY_HPP_


#include <Arduino.h>
#include "config.hpp"

#include <stdarg.h>
#include "sharp.hpp"


/**
 * @brief This class represents an array of sharps
 * 
 * All sharps on one side are grouped to one array.
 * 
 * @tparam N the amount of sharps in this array
 */
template<int N>
class SharpArray
{
    public:

        /**
         * @brief The constructor of the SharpArray class
         * 
         * This constructor stores a pointer to the ADC such that this
         * class will later be able to create new sharp objects.
         * 
         * @param _adc a pointer to a adc object
         */
        SharpArray(AnalogDigitalConverter *_adc);

        /**
         * @brief The function that creates a new pointer to a sharp
         * and returns a pointer to it
         * 
         * @param i the index of the sharp
         * @param _channel the channel of the pin the sharp is connected to
         * @return Sharp* a pointer to the created sharp
         */
        Sharp* setSharp(uint8_t i, uint8_t _channel);

        /**
         * @brief The function that returns a pointer to a specified sharp
         * 
         * @param i the index of the sharp
         * @return Sharp* the pointer to the sharp
         */
        Sharp* getSharp(uint8_t i);

        /**
         * @brief This function checks wheater all sharps detect a wall
         * 
         * @return true all sharps have detected a wall
         * @return false not all sharps have detected a wall
         */
        bool entireWall(void);

        /**
         * @brief The function that calculates the average measured distance for all sharps
         * 
         * @return int16_t the average distance (-1, if no wall detected)
         */
        int16_t average(void);

        /**
         * @brief The function that calculates the misalignment between sharp array and wall
         * 
         * This function calculates the misalignment by calculating the difference between
         * the first and last sharp
         * 
         * @return int16_t the difference between first and last sharp (-1, if no wall detected)
         */
        int16_t misalignment(void);

    private:

        /**
         * @brief A pointer to a adc object
         * 
         */
        AnalogDigitalConverter *adc;
        
        /**
         * @brief An array of pointers to the sharps
         */
        Sharp *sharp[N];
};

#endif
