#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "analog.h"
#include "black.h"
#include "drive.h"
#include "encoder.h"
#include "melexis.h"
#include "motor.h"
#include "navigation.h"
#include "rgb.h"
#include "switch.h"
#include "timer.h"
#include "victim.h"
#ifdef __cplusplus
}
#endif

void setup() {
    Serial.begin(19200);
    // init everything
    analogInit();
    blackInit();
    encoderInit();
    melexisInit();
    motorInit();
    navigationInit();
    rgbInit();
    switchInit();
    victimInit();
    // timer
    timerInit();
    rgbSet(32, 0, 0, 0);
    delay(400);
    rgbSet(0, 32, 0, 0);
}

void loop() {
    if(toggleswitch[0].value) {
        victimRecognition();
        if(enableNavigation) {
            navigationRightWall();
        }
    } else {
        rgbSet(128, 0, 0, 0);
        motorBrake();
    }
}
