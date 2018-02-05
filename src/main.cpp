/* 
 * main.cpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */ 


#include <Arduino.h>
#include "config.hpp"

#include "timer.hpp"

#include "pwm.hpp"
#include "motor.hpp"

Timer *t;
Motor *m;

/**
 * @brief Arduino setup function
 * 
 * This is the default Arduino setup function that gets called only once.
 */
void setup(void)
{
    t = new Timer(8, 1999, 50);

    Pwm *p = new Pwm(&PORTG, 5, 0, 2);
    m = new Motor(&PORTC, 4, 6, p->getDutycycleRegister(), &PINE, 5, &PINA, 7);
    delete p;

    m->setVelocity(255);

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
}

/**
 * @brief Timer loop
 * 
 * This is the timer loop that interrupts the main loop at a 1kHz rate.
 * The sensor read-ins and high power led are getting controlled here.
 */
ISR(TIMER1_COMPA_vect)
{
    m->update();
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
