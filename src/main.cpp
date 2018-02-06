/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"

#include "actuatorConfig.hpp"
#include "interfaceConfig.hpp"


Timer *t;

MotorController *motorController;

ToggleSwitch *toggleSwitch[3];
Button *button[5];
RGB *rgb;
Led *led;


/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Serial.begin(115200);

    t = new Timer(8, 1999, 50);

    actuatorInit();
    interfaceInit();
}

/**
 * @brief Arduino loop function
 * 
 * This is the default Arduino loop function that gets called until the Arduino
 * gets turned off.
 */
void loop(void)
{
    interfaceUpdate();

    uint8_t r=0;
    uint8_t g=0;
    uint8_t b=0;

    if(toggleSwitch[TOGGLE_RIGHT]->getState())
        r=32;
    if(toggleSwitch[TOGGLE_CENTER]->getState())
        g=32;
    if(toggleSwitch[TOGGLE_LEFT]->getState())
        b=32;

    rgb->set(r, g, b);
}

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER5_COMPA_vect)
{
    switch(t->loopInc())
    {
    case 10:
        led->set(255);
        break;
    case 20:
        led->set(0);
        break;
    }
}
