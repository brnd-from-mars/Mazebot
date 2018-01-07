#include <Arduino.h>


extern "C" void serialPrintTime() {
    Serial.println(millis());
}

extern "C" void serialPrintInt(int number) {
    Serial.println(number);
}

extern "C" void serialPrintNL() {
    Serial.println();
}
