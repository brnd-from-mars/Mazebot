#include "driveStateMachine.h"

void driveSMInit() {
    rotateState = -1;
    forwardState = -1;
    targetEncoderValue = 0;
}

void startRotate(int angle) {
    switch(angle) {
    case 90:
        rotateState = 0;
        targetEncoderValue = 75;
        break;
    case -90:
        rotateState = 0;
        targetEncoderValue = -75;
        break;
    case 180:
        rotateState = 0;
        targetEncoderValue = 150;
        break;
    case -180:
        rotateState = 0;
        targetEncoderValue = -150;
        break;
    }
}

void startForwardEnc(int distance) {
    targetEncoderValue = distance;
    forwardState = 0;
}

void startForwardCM(int distance) {
    startForwardEnc((int)(distance * 4.40));
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
            int speed=140;
            if (distanceCoveredEnc() > (int)(0.5*abs(targetEncoderValue)))
                speed = (int)(1.5*speed - speed*distanceCoveredEnc()/abs(targetEncoderValue));
            
            rgbSet(0, 32, 0, 0);
            rotate(SIGNUM(targetEncoderValue)*(uint8_t)(speed));
        }
        break;
    // correct position and orientation
    case 2:
        if(!correctRotationPosition(false)) {
            targetEncoderValue = 0;
            rotateState = -1;
        }
        rgbSet(0, 0, 32, 0);
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
            drive(SIGNUM(targetEncoderValue)*(uint8_t)(speed), 0.5, 0.02, 1.0);
        }
        break;
    // correct position and orientation
    case 2:
        if(!correctRotationPosition(false)) {
            targetEncoderValue = 0;
            forwardState = -1;
        }
        rgbSet(0, 0, 32, 0);
        break;
    }
}


int distanceCoveredEnc() {
    int sum=0;
    for(uint8_t i=0; i<4; i++)
        sum+=abs(encoder[i].value);
    
    return (int)(0.25*sum);
}
