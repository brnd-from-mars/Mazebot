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
    motor[FRONT_RIGHT] = _fr;
    motor[BACK_RIGHT]  = _br;
    motor[BACK_LEFT]   = _bl;
    motor[FRONT_LEFT]  = _fl;

    // calculate velocity factor for error calibration
}

void MotorController::setVelocityRight(int16_t velocity)
{
    motor[FRONT_RIGHT]->setVelocity(velocity);
    motor[BACK_RIGHT]->setVelocity(velocity);
}

void MotorController::setVelocityLeft(int16_t velocity)
{
    motor[FRONT_LEFT]->setVelocity(velocity);
    motor[BACK_LEFT]->setVelocity(velocity);
}

void MotorController::brake(void)
{
    for(uint8_t i=0; i<4; i++)
        motor[i]->brake();
}

void MotorController::update(void)
{
    for(uint8_t i=0; i<4; i++)
        motor[i]->update();
}

long MotorController::getAverageEncoderSteps(void)
{
    long sum;

    for(uint8_t i=0; i<4; i++)
        sum += abs(motor[i]->getSteps());

    return (long)(sum / 4);
}

void MotorController::reset(void)
{
    for(uint8_t i=0; i<4; i++)
        motor[i]->reset();
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
