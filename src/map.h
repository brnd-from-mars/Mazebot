#ifndef MAP_H_
#define MAP_H_

#include <Arduino.h>
#include "config.h"
#include "analog.h"


struct Point {
    int8_t x;
    int8_t y;
};

struct Field {
    /*
     * by value
     * 0 no
     * 1 yes
     */
    uint8_t visited : 1;

    /*
     * by value
     * 0 white
     * 1 black
     * 2 silver
     * 3 rampup
     * 4 rampdown
     * 5 unused
     * 6 unused
     * 7 unused
     */
    uint8_t type : 3;

    /*
     * bitwise
     * 
     * 3       2       1       0
     * NORTH   WEST    SOUTH   EAST
     */
    uint8_t wallData : 4;
};

struct Floor {
    struct Field fields[FLOOR_SIZE][FLOOR_SIZE];
};

struct Floor arena[FLOOR_COUNT];

struct Floor backup[FLOOR_COUNT];

struct Floor *currentFloor;

struct Field *currentField;

struct Point pos;

uint8_t heading;

void mapInit();

void mapCreatorForward();

bool mapCreatorRotate(int8_t amount);


#endif
