#ifndef RGB_H_
#define RGB_H_

#include <Arduino.h>


struct RGB {
    uint8_t pinR;
    uint8_t pinG;
    uint8_t pinB;
} rgb[1];

void rgbInit();

void rgbSet(uint8_t r, uint8_t g, uint8_t b, uint8_t i=0);

void rgbOff(uint8_t i=0);

#endif
