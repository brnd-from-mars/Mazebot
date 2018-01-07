#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "black.h"
#include "motor.h"
#include "rgb.h"
#include "timer.h"
#ifdef __cplusplus
}
#endif

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    blackInit();
    motorInit();
    rgbInit();
    timerInit();
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("Hello, world");
}
