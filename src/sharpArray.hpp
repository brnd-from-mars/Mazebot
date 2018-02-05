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
         * @brief A constructor of the class
         * 
         * This constructor creates N new sharps and stores the pointers to them.
         * 
         * @param _adc a pointer to a adc object
         * @param _sharpChannels the multiplexer channels the sharp is connected to
         */
        SharpArray(AnalogDigitalConverter *_adc, uint8_t _sharpChannels[N]);

        /**
         * @brief A constructor of the class
         * 
         * This constructor stores pointer to sharps created outside of the class.
         * 
         * @param _sharp an array of pointers to the sharps
         */
        SharpArray(Sharp *_sharp[N]);

        /**
         * @brief This function returns a pointer to the specified sharp
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
        bool entireWall();

    private:

        /**
         * @brief An array of pointers to the sharps
         */
        Sharp *sharp[N];
};

#endif
