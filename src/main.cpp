#include <Arduino.h>
#include <Servo.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "analog.h"
#include "drive.h"
#include "driveStateMachine.h"
#include "encoder.h"
#include "light.h"
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
    TIMER_STOP;
    lightMeasure();
    kitdropper.attach(11);
    kitdropper.write(SERVO_SHORT_LEFT);
    delay(1000);
    kitdropper.write(SERVO_SHORT_RIGHT);
    delay(400);
    kitdropper.write(SERVO_MIDDLE);
    delay(400);
    kitdropper.detach();
    TIMER_START;
}

void servoLeft() {
    TIMER_STOP;
    lightMeasure();
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
    TIMER_START;
}

void servoRight() {
    TIMER_STOP;
    lightMeasure();
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
    TIMER_START;
}

bool lastMapRestoreBackupSwitchState;

void setup() {
    Serial.begin(38400);
    Serial3.begin(38400);

    // init everything
    analogInit();
    driveSMInit();
    encoderInit();
    lightInit();
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

    lastMapRestoreBackupSwitchState = false;
}

void loop() {
    TIMER_STOP;
    rampInterrupt();
    melexisInterrupt();
    TIMER_START;

    if(toggleswitch[1].value) {

        if(toggleswitch[0].value) {

            victimRecognition();
            if(enableNavigation) {
                navigate();
            }

        } else {

            motorBrake();
            rgbSet(64, 32, 0, 0);

        }
        lastMapRestoreBackupSwitchState = true;

    } else {

        if(lastMapRestoreBackupSwitchState) {

            mapRestoreFromBackup();
            Serial.println(255);
            mapSender();
            encoderReset();
            driveSMInit();
            lastAction = NAVIGATION_ACTION_RESTORE_BKUP;

        }

        motorBrake();
        rgbSet(64, 32, 0, 0);
        lastMapRestoreBackupSwitchState = false;

    }

    rgbUpdate();

    if(victimSetKitdropper == 1) {
        servoRight();
        victimSetKitdropper = 0;
    }

    if(victimSetKitdropper == -1) {
        servoLeft();
        victimSetKitdropper = 0;
    }
}
