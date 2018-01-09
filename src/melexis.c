#include "melexis.h"


void melexisInit() {
    pinMode(SDA, INPUT_PULLUP);
    pinMode(SCL, INPUT_PULLUP);
    i2c_init();
}