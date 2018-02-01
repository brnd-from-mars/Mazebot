#include <Arduino.h>
#include <Servo.h>
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


Servo kitdropper;

void servoInit() {
    TIMER_STOP
    blackMeasure();
    kitdropper.attach(11);
    kitdropper.write(SERVO_SHORT_LEFT);
    delay(1000);
    kitdropper.write(SERVO_SHORT_RIGHT);
    delay(400);
    kitdropper.write(SERVO_MIDDLE);
    delay(400);
    kitdropper.detach();
    TIMER_START
}

void servoLeft() {
    TIMER_STOP
    blackMeasure();
    kitdropper.attach(11);
    kitdropper.write(SERVO_SHORT_LEFT);
    delay(400);
    kitdropper.write(SERVO_LEFT);
    delay(1000);
    kitdropper.write(SERVO_SHORT_LEFT);
    delay(1000);
    kitdropper.write(SERVO_MIDDLE);
    delay(400);
    kitdropper.detach();
    TIMER_START
}

void servoRight() {
    TIMER_STOP
    blackMeasure();
    kitdropper.attach(11);
    kitdropper.write(SERVO_SHORT_RIGHT);
    delay(400);
    kitdropper.write(SERVO_RIGHT);
    delay(1000);
    kitdropper.write(SERVO_SHORT_RIGHT);
    delay(1000);
    kitdropper.write(SERVO_MIDDLE);
    delay(400);
    kitdropper.detach();
    TIMER_START
}

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

    // servo
    servoLeft();
}

void loop() {
    if(toggleswitch[0].value) {
        victimRecognition();
        if(enableNavigation) {
            navigationRightWall();
        }
    } else {
        rgbOff(0);
        motorBrake();
    }

    if(toggleswitch[1].value) {
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

    if(victimSetKitdropper == 1)
        servoRight();
    if(victimSetKitdropper == 3)
        servoLeft();
}
