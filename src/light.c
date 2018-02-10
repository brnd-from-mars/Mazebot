#include "light.h"


void lightInit() {
    pinMode(A9, INPUT); // right
    pinMode(A10, INPUT); // left
    pinMode(24, OUTPUT);
    digitalWrite(24, LOW);
    
    darknessLeft = 0;
    darknessRight = 0;

    hiPoLedActive = false;

    isBlack = false;
    isSilver = false;
}


void lightPrepare() {
    digitalWrite(24, HIGH);
    hiPoLedActive = true;
}

void lightMeasure() {
    if(hiPoLedActive) {
        darknessLeft = analogRead(A10);
        darknessRight = analogRead(A9);
    }

    isBlack = ((darknessLeft > BLACK_MIN) || (darknessRight > BLACK_MIN));
    isSilver = (darknessRight < SILVER_MAX);

    digitalWrite(24, LOW);
    hiPoLedActive = false;
}
