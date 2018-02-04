/*
 * main.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include <Arduino.h>
#include "motor.hpp"
#include "pwm.hpp"


/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    pwm p = pwm(&PORTG, 5, 0, 2, &OCR0B);
    motor m = motor(&PORTC, 4, 6, &p);

    m.setVelocity(255);
}

/**
 * @brief Arduino loop function
 * 
 * This is the default Arduino loop function that gets called until the Arduino
 * gets turned off.
 */
void loop(void)
{
}
