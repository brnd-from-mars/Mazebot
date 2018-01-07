#include "rgb.h"


struct RGB rgb[1];

void rgbInit() {
    rgb[0].pinR = 8;
    rgb[0].pinG = 9;
    rgb[0].pinB = 10;

    for(uint8_t i=0; i<1; i++) {
        pinMode(rgb[i].pinR, OUTPUT);
        pinMode(rgb[i].pinG, OUTPUT);
        pinMode(rgb[i].pinB, OUTPUT);
    }
}

void rgbSet(uint8_t r, uint8_t g, uint8_t b, uint8_t i) {
    analogWrite(rgb[i].pinR, r);
    analogWrite(rgb[i].pinG, g);
    analogWrite(rgb[i].pinB, b);
}

void rgbOff(uint8_t i) {
    rgbSet(0, 0, 0, i);
}
