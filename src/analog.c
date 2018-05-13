#include "analog.h"


void analogInit() {
    sharp[0].pin = A11;
    sharp[1].pin = A0;
    sharp[2].pin = A15;
    sharp[3].pin = A1;
    sharp[4].pin = A4;
    sharp[5].pin = A6;
    sharp[6].pin = A7;
    sharp[7].pin = A5;
    sharp[8].pin = A8;

    for(uint8_t i=0; i<9; i++) {
        sharp[i].value = 0;
        pinMode(sharp[i].pin, INPUT);
    }

    nextSharp = 0;
}

void analogInterrupt() {
    long sum = 0;

    for(uint8_t i=0; i<3; i++)
        sum += trunc(analogRead(sharp[nextSharp].pin)/3);

    if(sum > 150 && sum < 450)
        sharp[nextSharp].value = (int16_t)(sum);
    else if(sum <= 150)
        sharp[nextSharp].value = 150;
    else
        sharp[nextSharp].value = 450;

    nextSharp = (nextSharp==8 ? 0 : nextSharp+1);
}

void analogVerify() {
    for(int i = 0; i < 9; i++) {
        analogInterrupt();
    }
}

bool entireWall(uint8_t dir, uint16_t maxDistance) {
    int sum = 0;
    switch(dir) {
    case RIGHT:
        return sharp[3].value>maxDistance && sharp[5].value>maxDistance;
    case BACK:
        return sharp[7].value>maxDistance && sharp[8].value>maxDistance;
    case LEFT:
        return sharp[4].value>maxDistance && sharp[6].value>maxDistance;
    case FRONT:
        for(uint8_t i = 0; i < 3; i++) {
            if(sharp[0].value > maxDistance)
                sum++;
            if(sum >= 2)
                return true;
        }
        break;
    }
    return false;
}

uint8_t getWallData(uint8_t hdg) {
    uint8_t wallData = 0;
    for(uint8_t local=0; local<4; local++) {
        if(entireWall(local, 150)) {
            uint8_t global = (local + hdg + 1)%4;
            wallData |= (1<<global);
        }
    }
    return wallData;
}

int8_t obstacleInFront() {

    int maxDistance = 180;

    // if wall in front
    if(entireWall(FRONT, maxDistance))
        return 0;

    // if object right but not left
    if((sharp[1].value > maxDistance) && (sharp[2].value < maxDistance))
        return 1;

    // if object left but not right
    if((sharp[1].value < maxDistance) && (sharp[2].value > maxDistance))
        return -1;

    return 0;
}

bool sharpBlockedFront(int distance) {

    for(int i = 0; i < 3; i++) {
        if(sharp[i].value > distance) {
            return true;
        }
    }

    return false;
}