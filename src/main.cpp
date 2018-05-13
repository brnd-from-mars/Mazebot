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

// i is for the imaginary constant
void servoI() {
    TIMER_STOP;
    lightMeasure();
    rgbSet(128, 0, 0, 0);
    delay(5000);
    rgbOff(0);
    TIMER_START;
}

bool lastMapRestoreBackupSwitchState;
String input;

void setup() {
    //Serial.begin(38400);
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

    input = "";

    while(digitalRead(40) == LOW);

    // map
    mapInit();

    // servo
    servoInit();

    Serial3.println("START");

    lastMapRestoreBackupSwitchState = false;
    Serial.begin(9600);
}

void(* resetFunc) (void) = 0;

void loop() {
    TIMER_STOP;
    rampLoop();
    melexisInterrupt();
    TIMER_START;

    Serial.println(darknessRight);

    if(Serial3.available() > 0) {
        //char* input = Serial3.readString().c_str();

        char tmp = Serial3.read();
        if(tmp == '\r' || tmp == '\n') {
            Serial.println("          " + input);

            int code = input.toInt();
            if(code != 0x000) {

                if((code & 0xF00) == 0xD00) {
                    if(((code & 0x0F0) == 0x000) || ((code & 0x0F0) == 0x010)) {
                        victimSetVisual(code);
                    }
                }
            }

            input = "";
        } else {
            input += tmp;
        }

    }

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
            encoderReset();
            driveSMInit();
            rampInit();
            lastAction = NAVIGATION_ACTION_RESTORE_BKUP;

        }

        motorBrake();
        rgbSet(64, 32, 0, 0);
        lastMapRestoreBackupSwitchState = false;

    }

    if(!toggleswitch[2].value && !toggleswitch[1].value && !toggleswitch[0].value) {
        resetFunc();
    //     mapReset();
    //     driveSMInit();
    //     encoderInit();
    //     lightInit();
    //     melexisInit();
    //     motorInit();
    //     navigationInit();
    //     rampInit();
    //     switchInit();
    //     victimInit();

    //     mapSender();
    }

    rgbUpdate();

    while(victimKitdropperLeft > 0) {
        motorBrake();
        rgbSet(128, 0, 0, 0);
        servoLeft();
        rgbOff(0);
        victimKitdropperLeft--;
    }

    while(victimKitdropperRight > 0) {
        motorBrake();
        rgbSet(128, 0, 0, 0);
        servoRight();
        rgbOff(0);
        victimKitdropperRight--;
    }

    while(victimKitdropperi > 0) {
        motorBrake();
        servoI();
        victimKitdropperi--;
    }
}
