#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include "config.h"
#include "analog.h"
#include "light.h"

#include <stdlib.h>


typedef struct Point {
    int8_t x : 4;
    int8_t y : 4;
} Point;


typedef struct Field {
    struct Field *neighbors[4];

    int8_t x : 4;
    int8_t y : 4;

    /*
     * 0 unvisited
     * 1 white
     * 2 black
     * 3 silver
     * 4 ramp
     * 5 unused
     * 6 unused
     * 7 unused
     */
    int8_t type;

    int8_t score;

    struct Field *next;
} Field;

typedef struct Floor {
    Field *start;
    Field *end;
} Floor;

typedef struct AdjacentScores {
    int8_t score[4];
} AdjacentScores;


uint8_t heading;

Floor *currentFloor;

Field *currentField;

void mapInit();

Field* mapCreateField(int8_t x, int8_t y, bool startField);

Point mapGetAdjacentPositionLocal(Point aP, uint8_t dir);

Point mapGetAdjacentPositionGlobal(Point aP, uint8_t dir);

Field* mapFindField(int8_t x, int8_t y);

void mapRotate(int8_t amount);

void mapForward();

void mapFrontFieldBlack();

void mapUpdate();

AdjacentScores mapGetAdjacentScores();

void mapSender();


#endif
