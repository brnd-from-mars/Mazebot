/*
 * interfaceConfig.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "interfaceConfig.hpp"


void interfaceInit()
{
    // toggle switch right
    toggleSwitch[TOGGLE_RIGHT] = new ToggleSwitch(&PINL, 5, false);

    // toggle switch center
    toggleSwitch[TOGGLE_CENTER] = new ToggleSwitch(&PINL, 7, false);

    // toggle switch left
    toggleSwitch[TOGGLE_LEFT] = new ToggleSwitch(&PING, 1, false);

    // button center
    button[BUTTON_CENTER] = new Button(&PINC, 1, true);

    // button up
    button[BUTTON_UP] = new Button(&PIND, 7, true);

    // button right
    button[BUTTON_RIGHT] = new Button(&PINC, 5, true);

    // button down
    button[BUTTON_DOWN] = new Button(&PINC, 7, true);

    // button left
    button[BUTTON_LEFT] = new Button(&PINC, 3, true);

    // rgb
    PwmPin pR = PwmPin(&PORTH, 5, 4, 3);
    PwmPin pG = PwmPin(&PORTH, 6, 2, 2);
    PwmPin pB = PwmPin(&PORTB, 4, 2, 1);
    rgb = new RGB(
        pR.getDutycycleRegister(),
        pG.getDutycycleRegister(),
        pB.getDutycycleRegister()
    );

    // led
    PwmPin pL = PwmPin(&PORTB, 7, 0, 1);
    led = new Led(pL.getDutycycleRegister());
}

void interfaceUpdate()
{
    for(uint8_t i=0; i<5; i++)
        button[i]->update();
}