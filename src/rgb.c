#include "rgb.h"


void rgbInit() {

    rgb[0].pinR = 8;
    rgb[0].pinG = 9;
    rgb[0].pinB = 10;

    for(uint8_t i=0; i<1; i++) {

        pinMode(rgb[i].pinR, OUTPUT);
        pinMode(rgb[i].pinG, OUTPUT);
        pinMode(rgb[i].pinB, OUTPUT);

        rgb[i].colorR = 0;
        rgb[i].colorG = 0;
        rgb[i].colorB = 0;

        rgb[i].currentlyBlinking = false;

    }
}

void rgbSet(uint8_t r, uint8_t g, uint8_t b, uint8_t i) {

    rgb[i].colorR = r;
    rgb[i].colorG = g;
    rgb[i].colorB = b;

    if(!rgb[i].currentlyBlinking) {

        analogWrite(rgb[i].pinR, r);
        analogWrite(rgb[i].pinG, g);
        analogWrite(rgb[i].pinB, b);

    }
}

void rgbBlink(uint8_t r, uint8_t g, uint8_t b, uint8_t i, long time) {

    rgb[i].currentlyBlinking = true;
    rgb[i].blinkingStop = millis() + time;
    
    analogWrite(rgb[i].pinR, r);
    analogWrite(rgb[i].pinG, g);
    analogWrite(rgb[i].pinB, b);
}

void rgbUpdate() {

    for(int i = 0; i < 1; i++) {

        if(rgb[i].currentlyBlinking) {

            if(millis() > rgb[i].blinkingStop) {
                rgb[i].currentlyBlinking = false;
            }

        } else {

            analogWrite(rgb[i].pinR, rgb[i].colorR);
            analogWrite(rgb[i].pinG, rgb[i].colorG);
            analogWrite(rgb[i].pinB, rgb[i].colorB);

        }
    }
}

void rgbOff(uint8_t i) {

    rgbSet(0, 0, 0, i);
}
