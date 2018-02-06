/*
 * motorController.cpp
 * created by Brendan Berg on 04.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


#include "motorController.hpp"


MotorController::MotorController(Motor *_fr, Motor *_br, Motor *_bl, Motor *_fl)
    : EepromCalibratable<uint16_t>::EepromCalibratable(0xA00, 4)
{
    // save pointers to all 4 motors
    motors[FRONT_RIGHT] = _fr;
    motors[BACK_RIGHT]  = _br;
    motors[BACK_LEFT]   = _bl;
    motors[FRONT_LEFT]  = _fl;

    // calculate velocity factor for error calibration
}

void MotorController::setVelocityRight(int16_t velocity)
{
    motors[FRONT_RIGHT]->setVelocity(velocity);
    motors[BACK_RIGHT]->setVelocity(velocity);
}

void MotorController::setVelocityLeft(int16_t velocity)
{
    motors[FRONT_LEFT]->setVelocity(velocity);
    motors[BACK_LEFT]->setVelocity(velocity);
}

void MotorController::startCalibration(void)
{
    // reset encoder values
    // start long term motor test
}

void MotorController::endCalibration(void)
{
    // write encoder values into eeprom
}
