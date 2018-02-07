/*
 * actuatorConfig.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef ACTUATORCONFIG_HPP_
#define ACTUATORCONFIG_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "pwmPin.hpp"
#include "motor.hpp"
#include "motorController.hpp"


/**
 * @brief The global MotorController that will get setup by actuatorInit
 */
extern MotorController *motorController;


/**
 * @brief The function that sets up the motors and a MotorController object
 */
void actuatorInit(void);


void actuatorUpdate(void);


#endif
