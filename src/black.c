#include "black.h"


void blackInit() {
    pinMode(A9, INPUT); // right
    pinMode(A10, INPUT); // left
    pinMode(24, OUTPUT);
    digitalWrite(24, LOW);

    darknessLeft = 0;
    darknessRight = 0;
}


void blackPrepare() {
    digitalWrite(24, HIGH);
    blackLedActive = true;
}

void blackMeasure() {
    if(blackLedActive) {
        darknessLeft = analogRead(A10);
        darknessRight = analogRead(A9);
    }

    digitalWrite(24, LOW);
    blackLedActive = false;
}
