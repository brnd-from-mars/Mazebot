/*
 * main.cpp
 * created by Brendan Berg on 03.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include <Arduino.h>
#include "config.hpp"

// #include "pwm.hpp"

// #include "motor.hpp"
// #include "motorController.hpp"

#include "timer.hpp"

#include "analogDigitalConverter.hpp"
#include "analogPin.hpp"


// MotorController *motorController;
Timer *timer;
AnalogDigitalConverter *adc;
AnalogPin *pin1, *pin2, *pin3;

/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Serial.begin(38400);

    // Pwm *p;
    // Motor *m[4];
    
    // p = new Pwm(&PORTG, 5, 0, 2);
    // m[FRONTRIGHT] = new Motor(&PORTC, 4, 6, p->getDutycycleRegister());
    // delete p;

    // p = new Pwm(&PORTE, 3, 3, 1);
    // m[BACKRIGHT] = new Motor(&PORTC, 2, 0, p->getDutycycleRegister());
    // delete p;

    // p = new Pwm(&PORTH, 3, 4, 1);
    // m[BACKLEFT] = new Motor(&PORTG, 2, 0, p->getDutycycleRegister());
    // delete p;

    // p = new Pwm(&PORTH, 4, 4, 2);
    // m[FRONTLEFT] = new Motor(&PORTL, 4, 6, p->getDutycycleRegister());
    // delete p;

    // motorController = new MotorController(m[FRONTRIGHT], m[BACKRIGHT], m[BACKLEFT], m[FRONTLEFT]);

    timer = new Timer(8, 1999); // 1kHz

    adc = new AnalogDigitalConverter();

    pin1 = new AnalogPin(adc, 0);
    pin2 = new AnalogPin(adc, 11);
    pin3 = new AnalogPin(adc, 15);
}

/**
 * @brief Arduino loop function
 * 
 * This is the default Arduino loop function that gets called until the Arduino
 * gets turned off.
 */
void loop(void)
{
    Serial.print(pin1->getLastValue());
    Serial.print(",");
    Serial.print(pin2->getLastValue());
    Serial.print(",");
    Serial.print(pin3->getLastValue());
    Serial.println();
}

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER1_COMPA_vect)
{
    pin1->read();
    pin2->read();
    pin3->read();
}
