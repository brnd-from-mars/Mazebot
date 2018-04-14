#ifndef RGB_H_
#define RGB_H_

#include <Arduino.h>


struct RGB {
    uint8_t pinR;
    uint8_t pinG;
    uint8_t pinB;

    uint8_t colorR;
    uint8_t colorG;
    uint8_t colorB;

    bool currentlyBlinking;
    long blinkingStop;
} rgb[1];

void rgbInit();

void rgbSet(uint8_t r, uint8_t g, uint8_t b, uint8_t i);

void rgbBlink(uint8_t r, uint8_t g, uint8_t b, uint8_t i, long time);

void rgbUpdate();

void rgbOff(uint8_t i);


#endif
