#include <Arduino.h>
#include "black.h"
#include "motor.h"
#include "rgb.h"
#include "timer.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(11520);
    blackInit();
    motorInit();
    rgbInit();
    timerInit();
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("Hello, world");
}
