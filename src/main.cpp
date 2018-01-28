#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "analog.h"
#include "black.h"
#include "drive.h"
#include "encoder.h"
#include "map.h"
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
    delay(400);
    // map
    mapInit();
}

void loop() {
    if(completed) {
        Serial.print("hdg: ");
        Serial.print(heading);
        Serial.print(" y: ");
        Serial.print(pos.y);
        Serial.print(" x: ");
        Serial.println(pos.x);
        for(uint8_t y=0; y<FLOOR_SIZE; y++) {
            for(uint8_t x=0; x<FLOOR_SIZE; x++) {
                Serial.print("y: ");
                Serial.print(y);
                Serial.print(" x: ");
                Serial.print(x);
                Serial.print(" visited: ");
                Serial.print(arena[0].fields[y][x].visited, BIN);
                Serial.print(" type ");
                Serial.print(arena[0].fields[y][x].type);
                Serial.print(" wallData: ");
                Serial.println(arena[0].fields[y][x].wallData, BIN);
            }
        }
        Serial.println();
    }

    if(toggleswitch[0].value) {
        victimRecognition();
        if(enableNavigation) {
            navigationRightWall();
        }
    } else {
        rgbOff(0);
        motorBrake();
    }
}
