/*
 * sensorConfig.hpp
 * created by Brendan Berg on 06.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#ifndef SENSORCONFIG_HPP_
#define SENSORCONFIG_HPP_


#include <Arduino.h>
#include "config.hpp"

#include "analogDigitalConverter.hpp"
#include "analogPin.hpp"
#include "ramp.hpp"
#include "light.hpp"
#include "sharp.hpp"
#include "sharpArray.hpp"
#include "melexis.hpp"


extern Ramp *ramp;

extern Light *light;

extern Sharp *sharp[9];

extern SharpArray<3> *saFront;
extern SharpArray<2> *saRight;
extern SharpArray<2> *saBack;
extern SharpArray<2> *saLeft;

extern Melexis *meRight;
extern Melexis *meLeft;


void sensorInit(void);

void sensorUpdateMain(void);

void sensorUpdateTimer(void);


#endif
