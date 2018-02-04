/*
 * motorController.cpp
 * created by Brendan Berg on 04.01.2018
 * 
 * Copyright (c) 2017 SiemBots
 */


#include "motorController.hpp"
#include "config.hpp"
#include "eepromCalibratable.cpp"


motorController::motorController(motor *_fr, motor *_br, motor *_bl, motor *_fl)
    : eepromCalibratable<uint16_t>::eepromCalibratable(0xA00, 4)
{
    // save pointers to all 4 motors
    motors[FRONTRIGHT] = _fr;
    motors[BACKRIGHT]  = _br;
    motors[BACKLEFT]   = _bl;
    motors[FRONTLEFT]  = _fl;

    // calculate velocity factor for error calibration
}

void motorController::setVelocityRight(int16_t velocity)
{
    motors[FRONTRIGHT]->setVelocity(velocity);
    motors[BACKRIGHT]->setVelocity(velocity);
}

void motorController::setVelocityLeft(int16_t velocity)
{
    motors[FRONTLEFT]->setVelocity(velocity);
    motors[BACKLEFT]->setVelocity(velocity);
}

void motorController::startCalibration(void)
{
    // reset encoder values
    // start long term motor test
}

void motorController::endCalibration(void)
{
    // write encoder values into eeprom
}