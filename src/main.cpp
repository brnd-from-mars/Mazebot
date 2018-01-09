#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "analog.h"
#include "black.h"
#include "encoder.h"
#include "melexis.h"
#include "motor.h"
#include "rgb.h"
#include "timer.h"
#ifdef __cplusplus
}
#endif


void setup() {
    Serial.begin(115200);
    // init everything
    analogInit();
    blackInit();
    encoderInit();
    melexisInit();
    motorInit();
    rgbInit();
    timerInit();
    // safty time
    rgbSet(32, 0, 0, 0);
    delay(200);
    rgbSet(0, 32, 0, 0);
}

void loop() {
}
