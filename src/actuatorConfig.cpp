/*
 * actuatorConfig.cpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "actuatorConfig.hpp"


void actuatorInit(void)
{
    // motor front right
    PwmPin pFR = PwmPin(&PORTG, 5, 0, 2);
    Motor *mFR = new Motor(&PORTC, 4, 6, pFR.getDutycycleRegister(), &PINE, 5, &PINA, 7);

    // motor back right
    PwmPin pBR = PwmPin(&PORTE, 3, 2, 1);
    Motor *mBR = new Motor(&PORTC, 2, 0, pBR.getDutycycleRegister(), &PINA, 3, &PIND, 3);

    // motor back left
    PwmPin pBL = PwmPin(&PORTH, 3, 4, 1);
    Motor *mBL = new Motor(&PORTG, 2, 0, pBL.getDutycycleRegister(), &PIND, 2, &PINL, 0);

    // motor front left
    PwmPin pFL = PwmPin(&PORTH, 4, 4, 2);
    Motor *mFL = new Motor(&PORTL, 4, 6, pFL.getDutycycleRegister(), &PINL, 2, &PINE, 4);

    motorController = new MotorController(mFR, mBR, mBL, mFL);
}

void actuatorUpdate(void)
{
    motorController->update();
}
