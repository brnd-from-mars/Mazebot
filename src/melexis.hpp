/*
 * melexis.hpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 Brendan Berg
 */


#ifndef MELEXIS_HPP_
#define MELEXIS_HPP_


#include <Arduino.h>
#include "config.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include <i2cmaster.h>
#ifdef __cplusplus
}
#endif


/**
 * @brief A class that represents a melexis sensor
 * 
 * This class represents a melexis infrared temperature sensor.
 * It executes the SMBus communication with the i2cmaster.h library
 * and converts the result to a practical unit.
 */
class Melexis
{
    public:

        /**
         * @brief The constructor of the Melexis class
         * 
         * The constructor stores the address and starts
         * the first measurement.
         * 
         * @param _address the address of the represented sensor
         */
        Melexis(uint8_t _address);

        /**
         * @brief The function that recieves and
         * computes the temperature
         * 
         * This method should get called very often because otherwise
         * this API will provide only access to outdated data.
         * This function should not get called inside the timer loop
         * because its runtime is not appropriate therefore.
         */
        void read(void);

        /**
         * @brief The function that returns the last measurement
         * 
         * @return float the temperature in celsius
         */
        float getTemperature(void);

    private:

        /**
         * @brief The function that converts the raw data to celsius
         * 
         * @param _raw the raw data
         * @return float the converted value
         */
        float convertToCelsius(uint16_t _raw);

        /**
         * @brief The address of the represented sensor
         */
        uint8_t address;

        /**
         * @brief The last measured value (not converted)
         */
        uint16_t temperatureRaw;

        /**
         * @brief Thelast measured value (converted)
         * 
         */
        float temperatureCelsius;

};


#endif
