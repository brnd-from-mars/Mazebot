/*
 * interfaceConfig.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef INTERFACEGUARD_HPP
#define INTERFACEGUARD_HPP


#include <Arduino.h>
#include "config.hpp"

#include "pwmPin.hpp"
#include "toggleSwitch.hpp"
#include "button.hpp"
#include "rgb.hpp"
#include "led.hpp"


/**
 * @brief The global ToggleSwitch array that will get setup by interfaceInit
 */
extern ToggleSwitch *toggleSwitch[3];

/**
 * @brief The global Button array that will get setup by interfaceInit
 */
extern Button *button[5];

/**
 * @brief The global RGB led that will get setup by interfaceInit
 */
extern RGB *rgb;

/**
 * @brief The global single-color led that will get setup by interfaceInit
 */
extern Led *led;


/**
 * @brief The function that sets up all interface elements
 */
void interfaceInit(void);

/**
 * @brief The function that updates the interface elements in every main-loop run-through
 * 
 */
void interfaceUpdate(void);


#endif
