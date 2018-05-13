#include "driveStateMachine.h"

void driveSMInit() {
    rotateState = -1;
    forwardState = -1;
    targetEncoderValue = 0;
    cameraStart = 0;
    moreThanHalfDriven = true;
    obstalceInFrontDetected = 0;
}

void startRotate(int angle, int speed) {
    rampDiscardBecauseRotation();
    rotationSpeed = speed;

    switch(angle) {
    case 90:
        rotateState = 0;
        targetEncoderValue = DRIVE_ENC_STEPS_PER_90_ROT;
        break;
    case -90:
        rotateState = 0;
        targetEncoderValue = -DRIVE_ENC_STEPS_PER_90_ROT;
        break;
    case 180:
        rotateState = 0;
        targetEncoderValue = 2*DRIVE_ENC_STEPS_PER_90_ROT;
        break;
    case -180:
        rotateState = 0;
        targetEncoderValue = -2*DRIVE_ENC_STEPS_PER_90_ROT;
        break;
    }
}

void startForwardEnc(int distance) {
    targetEncoderValue = distance;
    forwardState = 0;
    moreThanHalfDriven = false;
    obstalceInFrontDetected = 0;
}

void startForwardCM(int distance) {
    startForwardEnc((int)(distance * DRIVE_ENC_STEPS_PER_CM));
}

void processRotate() {
    switch(rotateState) {
    // do not perform, this case should never get called
    case -1:
        motorBrake();
        targetEncoderValue = 0;
        break;
    // start rotation
    case 0:
        motorBrake();
        encoderReset();
        rotateState = 1;
        break;
    // perform rotation
    case 1:
        if(distanceCoveredEnc() >= abs(targetEncoderValue)) {
            correctRotationPosition(true, true);
            rotateState = 2;
        } else {
            rgbSet(0, 32, 0, 0);
            rotate(SIGNUM(targetEncoderValue)*(uint8_t)(rotationSpeed));
        }
        break;
    // correct position and orientation
    case 2:
        if(!correctRotationPosition(false, true)) {
            motorBrake();
            driveReset();
            targetEncoderValue = 0;
            cameraStart = millis();
            sendScanningCommand();
            rotateState = 3;
        }
        rgbSet(0, 0, 32, 0);
        break;
    // wait until image was taken
    case 3:
        if(millis() > cameraStart + 1500) {
            rotateState = -1;
        }
        rgbSet(255, 255, 255, 0);
        motorBrake();
        break;
    }
}

void processForward() {
    switch(forwardState) {
    // do not perform, this case should never get called
    case -1:
        motorBrake();
        targetEncoderValue = 0;
        break;
    // start forward drive
    case 0:
        motorBrake();
        encoderReset();
        forwardState = 1;
        break;
    case 1:
        if(obstacleInFront() != 0)
            obstalceInFrontDetected = obstacleInFront();

        if(entireWall(FRONT, 300)) {
            motorBrake();
            analogVerify();
            fallbackVerifyTimer = millis();
            forwardState = 4;
        } else if(distanceCoveredEnc() >= abs(targetEncoderValue)) {
            driveReset();
            correctRotationPosition(true, true);
            forwardState = 2;
        } else {
            // abort
            // if((targetEncoderValue > 0) && (sharp[0].value > 280)) {
            //     forwardState = 2;
            // }
            int speed = 140;
            rgbSet(0, 32, 0, 0);
            drive(SIGNUM(targetEncoderValue)*(uint8_t)(speed), 0.5, 0.02, 0.7);
        }
        break;
    // correct position and orientation
    case 2:
        moreThanHalfDriven = getForwardProcess(0.5);
        if(!correctRotationPosition(false, true)) {
            motorBrake();
            driveReset();
            targetEncoderValue = 0;
            cameraStart = millis();
            sendScanningCommand();
            forwardState = 3;
        }
        rgbSet(0, 0, 32, 0);
        break;
    // wait until image was taken
    case 3:
        if(millis() > cameraStart + 1500) {
            forwardState = -1;
        }
        rgbSet(255, 255, 255, 0);
        motorBrake();
        break;
    // maybe wall detected
    case 4:
        if(millis() > fallbackVerifyTimer + 20) {
            if(entireWall(FRONT, 300)) {
                driveReset();
                correctRotationPosition(true, (obstalceInFrontDetected == 0));
                forwardState = 2;
            } else {
                forwardState = 1;
            }
        }
        break;
    }
}

void sendScanningCommand() {
    if(entireWall(RIGHT, 150)) {
        serialPrintInt(COM_SCAN_RIGHT);
        serialPrintNL();
    }
    if(entireWall(LEFT, 150)) {
        serialPrintInt(COM_SCAN_LEFT);
        serialPrintNL();
    }
}

int distanceCoveredEnc() {
    int sum=0;
    for(uint8_t i=0; i<4; i++)
        sum+=abs(encoder[i].value);
    
    return (int)(0.25*sum);
}

int getRotationProcess() {
    if(rotateState != -1)
        if(abs(distanceCoveredEnc())>abs(targetEncoderValue)/2)
            return SIGNUM(targetEncoderValue);

    return 0;
}

int getForwardProcess(float threshold) {
    if((forwardState != -1) && (rampState == 0))
        if(SIGNUM(targetEncoderValue)==1)
            if(abs(distanceCoveredEnc()) > threshold * abs(targetEncoderValue))
                return 1;

    if(forwardState == -1)
        return 1;
    
    return 0;
}
