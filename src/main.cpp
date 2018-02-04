/*
 * main.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include <Arduino.h>
#include "config.hpp"
#include "motor.hpp"
#include "motorController.hpp"
#include "pwm.hpp"


motorController *mController;


/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Serial.begin(38400);

    pwm *p;
    motor *m[4];
    
    p = new pwm(&PORTG, 5, 0, 2);
    m[FRONTRIGHT] = new motor(&PORTC, 4, 6, p->getDutycycleRegister());
    delete p;

    p = new pwm(&PORTE, 3, 3, 1);
    m[BACKRIGHT] = new motor(&PORTC, 2, 0, p->getDutycycleRegister());
    delete p;

    p = new pwm(&PORTH, 3, 4, 1);
    m[BACKLEFT] = new motor(&PORTG, 2, 0, p->getDutycycleRegister());
    delete p;

    p = new pwm(&PORTH, 4, 4, 2);
    m[FRONTLEFT] = new motor(&PORTL, 4, 6, p->getDutycycleRegister());
    delete p;

    mController = new motorController(m[FRONTRIGHT], m[BACKRIGHT], m[BACKLEFT], m[FRONTLEFT]);
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
