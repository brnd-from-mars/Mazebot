#include "encoder.h"


void encoderInit() {
    encoder[0].pin1 = 3;
    encoder[0].pin2 = 29;
    encoder[0].forward = HIGH;
    encoder[0].value = 0;
    attachInterrupt(digitalPinToInterrupt(encoder[0].pin1), encoder0, RISING);

    encoder[1].pin1 = 18;
    encoder[1].pin2 = 25;
    encoder[1].forward = LOW;
    encoder[1].value = 0;
    attachInterrupt(digitalPinToInterrupt(encoder[1].pin1), encoder1, RISING);

    encoder[2].pin1 = 2;
    encoder[2].pin2 = 47;
    encoder[2].forward = LOW;
    encoder[2].value = 0;
    attachInterrupt(digitalPinToInterrupt(encoder[2].pin1), encoder2, RISING);

    encoder[3].pin1 = 19;
    encoder[3].pin2 = 49;
    encoder[3].forward = HIGH;
    encoder[3].value = 0;
    attachInterrupt(digitalPinToInterrupt(encoder[3].pin1), encoder3, RISING);
}

void encoderReset() {
    for(uint8_t i=0; i<4; i++) {
        encoder[i].value = 0;
    }
}

void encoder0() {
	if(digitalRead(encoder[0].pin2) == encoder[0].forward) {
        encoder[0].value++;
    }
	else {
        encoder[0].value--;
    }
}

void encoder1() {
	if(digitalRead(encoder[1].pin2) == encoder[1].forward) {
        encoder[1].value++;
    }
	else {
        encoder[1].value--;
    }
}

void encoder2() {
	if(digitalRead(encoder[2].pin2) == encoder[2].forward) {
        encoder[2].value++;
    }
	else {
        encoder[2].value--;
    }
}

void encoder3() {
	if(digitalRead(encoder[3].pin2) == encoder[3].forward) {
        encoder[3].value++;
    }
	else {
        encoder[3].value--;
    }
}
