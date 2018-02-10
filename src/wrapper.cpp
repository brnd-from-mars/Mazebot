#include <Arduino.h>
#include "config.h"


#define BLUETOOTH

extern "C" void serialPrintTime() {
    Serial.print(millis());
    Serial.print(" ");
#ifdef BLUETOOTH
    Serial3.print(millis());
    Serial3.print(" ");
#endif
}
extern "C" void serialPrintInt(int number) {
    Serial.print(number);
    Serial.print(" ");
#ifdef BLUETOOTH
    Serial3.print(number);
    Serial3.print(" ");
#endif
}

extern "C" void serialPrintFloat(float number) {
    Serial.print(number);
    Serial.print(" ");
#ifdef BLUETOOTH
    Serial3.print(number);
    Serial3.print(" ");
#endif
}

extern "C" void serialPrintNL() {
    Serial.println();
#ifdef BLUETOOTH
    Serial3.println();
#endif
}

extern "C" void serialMap(uint8_t x, uint8_t y, uint8_t wallData) {
    Serial.print("mx");
    Serial.print(x);
    Serial.print("y");
    Serial.print(y);
    Serial.print("n");
    Serial.print((wallData&(1<<NORTH))>>NORTH);
    Serial.print("e");
    Serial.print((wallData&(1<<EAST))>>EAST);
    Serial.print("s");
    Serial.print((wallData&(1<<SOUTH))>>SOUTH);
    Serial.print("w");
    Serial.print((wallData&(1<<WEST))>>WEST);
    Serial.println("M");
#ifdef BLUETOOTH
    Serial3.print("mx");
    Serial3.print(x);
    Serial3.print("y");
    Serial3.print(y);
    Serial3.print("n");
    Serial3.print((wallData&(1<<NORTH))>>NORTH);
    Serial3.print("e");
    Serial3.print((wallData&(1<<EAST))>>EAST);
    Serial3.print("s");
    Serial3.print((wallData&(1<<SOUTH))>>SOUTH);
    Serial3.print("w");
    Serial3.print((wallData&(1<<WEST))>>WEST);
    Serial3.println("M");
#endif
}
