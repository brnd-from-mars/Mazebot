/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"

#include "sensorConfig.hpp"
#include "interfaceConfig.hpp"
#include "actuatorConfig.hpp"


Timer *t;

Ramp *ramp;

Light *light;

Sharp *sharp[9];

SharpArray<3> *saFront;
SharpArray<2> *saRight;
SharpArray<2> *saBack;
SharpArray<2> *saLeft;

Melexis *meLeft;
Melexis *meRight;

ToggleSwitch *toggleSwitch[3];
Button *button[5];
RGB *rgb;
Led *led;

MotorController *motorController;


uint8_t s;

/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Serial.begin(57600);

    t = new Timer(8, 1999);

    s = 0;

    sensorInit();
    interfaceInit();
    actuatorInit();
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
    sensorUpdateMain();
}

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER5_COMPA_vect)
{
    sensorUpdateTimer();
    actuatorUpdate();
}
