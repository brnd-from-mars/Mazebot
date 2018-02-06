/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"

#include "toggleSwitch.hpp"
#include "button.hpp"

#include "pwmPin.hpp"
#include "rgb.hpp"
#include "led.hpp"

Timer *t;

Button *b;

ToggleSwitch *ts;

Led *led;

/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    Serial.begin(115200);

    DDRB |= (1<<7);

    t = new Timer(8, 1999, 50);

    PwmPin *pr = new PwmPin(&PORTH, 5, 4, 3);
    PwmPin *pg = new PwmPin(&PORTH, 6, 2, 2);
    PwmPin *pb = new PwmPin(&PORTB, 4, 2, 1);

    RGB rgb = RGB(pr->getDutycycleRegister(), pg->getDutycycleRegister(), pb->getDutycycleRegister());

    PwmPin *pl = new PwmPin(&PORTB, 7, 0, 1);

    led = new Led(pl->getDutycycleRegister());

    rgb.set(0, 0, 64);
}

/**
 * @brief Arduino loop function
 * 
 * This is the default Arduino loop function that gets called until the Arduino
 * gets turned off.
 */
void loop(void)
{
    b->update();
    Serial.print(ts->getState());
    Serial.print(",");
    Serial.println(b->getState());
    delay(10);
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
