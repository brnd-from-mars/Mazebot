/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"


/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Timer t = Timer(8, 1999);
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

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER1_COMPA_vect)
{
}
