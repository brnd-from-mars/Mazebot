#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include "config.h"
#include "analog.h"

#include <stdlib.h>


int memoryCounter;

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
} Field;

typedef struct Floor {
    Field *start;
} Floor;

typedef struct FieldLinkedListElement {
    Field *field;

    struct FieldLinkedListElement *prev;
    struct FieldLinkedListElement *next;
} FieldLinkedListElement;

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

FieldLinkedListElement *mapFloorTo1DList();

Field* mapFindField(int8_t x, int8_t y);

void mapFreeLinkedList(FieldLinkedListElement *fieldPtr);

void mapRotate(int8_t amount);

void mapForward();

void mapUpdate();

AdjacentScores mapGetAdjacentScores();

void mapSender();


#endif
