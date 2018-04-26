#include "driveStateMachine.h"

void driveSMInit() {
    rotateState = -1;
    forwardState = -1;
    targetEncoderValue = 0;
    cameraStart = 0;
}

void startRotate(int angle) {
    switch(angle) {
    case 90:
        rotateState = 0;
        targetEncoderValue = 70;
        break;
    case -90:
        rotateState = 0;
        targetEncoderValue = -70;
        break;
    case 180:
        rotateState = 0;
        targetEncoderValue = 140;
        break;
    case -180:
        rotateState = 0;
        targetEncoderValue = -140;
        break;
    }
}

void startForwardEnc(int distance) {
    targetEncoderValue = distance;
    forwardState = 0;
}

void startForwardCM(int distance) {
    startForwardEnc((int)(distance * 4.70));
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
            correctRotationPosition(true);
            rotateState = 2;
        } else {
            int speed=160;
            rgbSet(0, 32, 0, 0);
            rotate(SIGNUM(targetEncoderValue)*(uint8_t)(speed));
        }
        break;
    // correct position and orientation
    case 2:
        if(!correctRotationPosition(false)) {
            motorBrake();
            driveReset();
            targetEncoderValue = 0;
            cameraStart = millis();

            if(entireWall(RIGHT, 150)) {
                serialPrintInt(COM_SCAN_RIGHT);
                serialPrintNL();
            }
            if(entireWall(LEFT, 150)) {
                serialPrintInt(COM_SCAN_LEFT);
                serialPrintNL();
            }
            rotateState = -1;
        }
        rgbSet(0, 0, 32, 0);
        break;
    // wait until image was taken
    case 3:
        if(millis() > cameraStart + 500) {
            rotateState = -1;
        }
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
        if(distanceCoveredEnc() >= abs(targetEncoderValue)) {
            driveReset();
            correctRotationPosition(true);
            forwardState = 2;
        } else {
            int speed = 140;
            if (distanceCoveredEnc() > (int)(0.5*abs(targetEncoderValue)))
                speed = (int)(1.5*speed - speed*distanceCoveredEnc()/abs(targetEncoderValue));
            
            rgbSet(0, 32, 0, 0);
            drive(SIGNUM(targetEncoderValue)*(uint8_t)(speed), 0.5, 0.02, 0.7);
        }
        break;
    // correct position and orientation
    case 2:
        if(!correctRotationPosition(false)) {
            motorBrake();
            driveReset();
            targetEncoderValue = 0;
            cameraStart = millis();

            if(entireWall(RIGHT, 150)) {
                serialPrintInt(COM_SCAN_RIGHT);
                serialPrintNL();
            }
            if(entireWall(LEFT, 150)) {
                serialPrintInt(COM_SCAN_LEFT);
                serialPrintNL();
            }
            forwardState = 3;
        }
        rgbSet(0, 0, 32, 0);
        break;
    // wait until image was taken
    case 3:
        if(millis() > cameraStart + 500) {
            forwardState = -1;
        }
        motorBrake();
        break;
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

int getForwardProcess() {
    if(forwardState != -1)
        if(SIGNUM(targetEncoderValue)==1)
            if(abs(distanceCoveredEnc())>0.7*abs(targetEncoderValue))
                return 1;

    return 0;
}
