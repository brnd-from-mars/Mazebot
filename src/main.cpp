/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"

#include "encoder.hpp"

Timer *t;
Encoder *e;

/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    t = new Timer(8, 1999, 50);

    e = new Encoder(&PINE, 5, &PINA, 7);

    Serial.begin(115200);
}

/**
 * @brief Arduino loop function
 * 
 * This is the default Arduino loop function that gets called until the Arduino
 * gets turned off.
 */
void loop(void)
{
    Serial.println(e->getSteps());
}

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER1_COMPA_vect)
{
    e->update();
    switch(t->loopInc())
    {
    case 10:
        PORTB |= (1<<7);
        break;
    case 20:
        PORTB &= ~(1<<7);
        break;
    }
}
