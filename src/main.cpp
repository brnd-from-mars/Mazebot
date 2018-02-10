#include <Arduino.h>
#include <Servo.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "analog.h"
#include "black.h"
#include "drive.h"
#include "driveStateMachine.h"
#include "encoder.h"
#include "map.h"
#include "melexis.h"
#include "motor.h"
#include "navigation.h"
#include "ramp.h"
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
    Serial.begin(38400);
    Serial3.begin(38400);

    // init everything
    analogInit();
    blackInit();
    driveSMInit();
    encoderInit();
    melexisInit();
    motorInit();
    navigationInit();
    rampInit();
    rgbInit();
    switchInit();
    victimInit();

    // timer
    timerInit();
    delay(400);

    // map
    mapInit();

    // servo
    servoInit();

    Serial3.println("START");
}

void loop() {
    melexisInterrupt();

    Serial.println(melexis[1].value);
    Serial.println(melexis[3].value);

    //Serial3.println();
    //Serial3.print("MC: ");
    //Serial3.println(memoryCounter);

    TIMER_STOP
    rampInterrupt();
    TIMER_START

    if(toggleswitch[0].value) {
        victimRecognition();
        if(enableNavigation)
            navigate();
    } else {
        rgbOff(0);
        motorBrake();
    }

    if(victimSetKitdropper == 1) {
        servoRight();
        victimSetKitdropper = 0;
    }

    if(victimSetKitdropper == -1) {
        servoLeft();
        victimSetKitdropper = 0;
    }
}
