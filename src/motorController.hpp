/*
 * motorController.hpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#ifndef MOTORCONTROLLER_HPP_
#define MOTORCONTROLLER_HPP_


#include <Arduino.h>
#include "motor.hpp"
#include "eepromCalibratable.hpp"


/**
 * @brief A class that controls 4 seperate motors.
 * 
 * This class controls 4 velocity and direction of 4 different motors,
 * handles the encoder values and calibrates the motors.
 */
class MotorController : protected EepromCalibratable<uint16_t>
{
    
    public:

        /**
         * @brief The constructor of the MotorControll class
         * 
         * This function saves the pointers to the 4 motor objects.
         * 
         * @param _fr a pointer to the front-right positioned motor
         * @param _br a pointer to the back-right positioned motor
         * @param _bl a pointer to the back-left positioned motor
         * @param _fl a pointer to the front-left positioned motor
         */
        MotorController(Motor *_fr, Motor *_br, Motor *_bl, Motor *_fl);

        /**
         * @brief A function that sets the velocity of both motors on the RHS
         * 
         * @param velocity the velocity between -255 (backwards) and 255 (forwards)
         */
        void setVelocityRight(int16_t velocity);

        /**
         * @brief A function that sets the velocity of both motors on the LHS
         * 
         * @param velocity the velocity between -255 (backwards) and 255 (forwards)
         */
        void setVelocityLeft(int16_t velocity);

        /**
         * @brief A function that starts the motor calibration process
         */
        void startCalibration(void);

        /**
         * @brief A function that ends the motor calibration process and saves results inside eeprom
         */
        void endCalibration(void);

    private:

        /**
         * @brief Pointers to all 4 motors
         */
        Motor *motors[4];

};


#endif