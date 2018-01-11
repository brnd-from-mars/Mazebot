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
#include "rgb.h"
#include "switch.h"
#include "timer.h"
#ifdef __cplusplus
}
#endif


void setup() {
    Serial.begin(115200);
    // init everything
    analogInit();
    blackInit();
    driveInit();
    encoderInit();
    melexisInit();
    motorInit();
    rgbInit();
    switchInit();
    timerInit();
    // safty time
    rgbSet(32, 0, 0, 0);
    delay(200);
    rgbSet(0, 32, 0, 0);
}

void loop() {
    if(toggleswitch[0].value) {
        drive(100, 0.5, 0.02, 1.0);
    } else {
        motorBrake();
        rgbOff(0);
    }
}
