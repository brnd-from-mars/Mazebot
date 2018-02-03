#include <Arduino.h>


extern "C" void serialPrintTime() {
    Serial.print(millis());
    Serial.print(" ");
}

extern "C" void serialPrintInt(int number) {
    Serial.print(number);
    Serial.print(" ");
}

extern "C" void serialPrintFloat(float number) {
    Serial.print(number);
    Serial.print(" ");
}

extern "C" void serialPrintNL() {
    Serial.println();
}
