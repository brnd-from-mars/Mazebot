#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include "config.h"
#include "analog.h"
#include "light.h"
#include "rgb.h"

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
    uint8_t id;

    Field *start;
    Field *end;

    Field *lastField;

    bool finished;

    struct Floor *next;
} Floor;

typedef struct AdjacentScores {
    int8_t score[4];
} AdjacentScores;

typedef struct Ramp {
    int8_t floor1;
    Point field1;

    int8_t floor2;
    Point field2;

    struct Ramp *next;
} Ramp;

typedef struct Victim {
    int8_t dir;
    int8_t floor;
    Point field;

    struct Victim *next;
} Victim;


uint8_t heading;

Floor *startFloor;

Floor *currentFloor;

Field *currentField;

Ramp *firstRamp;

Victim *firstVictim;

bool blockRampUp;

bool blockRampDown;

// BACKUP:

uint8_t bkupHeading;

Floor *bkupStartFloor;

Floor *bkupCurrentFloor;

Field *bkupCurrentField;

Ramp *bkupFirstRamp;

Victim *bkupFirstVictim;

bool bkupBlockRampUp;

bool bkupBlockRampDown;

// FUNCTIONS:

void mapInit();

Field* mapCreateField(int8_t x, int8_t y, bool startField);

uint8_t mapLocalToGlobalDirection(uint8_t local);

Point mapGetAdjacentPositionLocal(Point aP, uint8_t dir);

Point mapGetAdjacentPositionGlobal(Point aP, uint8_t dir);

Field* mapFindField(int8_t x, int8_t y);

void mapRotate(int8_t amount);

void mapForward(bool ramp);

void mapFrontFieldBlack();

void mapSetRamp();

void mapFinishRamp();

bool mapJustFinishedRamp();

void mapSetVictim(int side, int rotOffset, bool front);

bool mapAlreadyVictimRecognized(int side, int rotOffset, bool front);

void mapUpdate();

AdjacentScores mapGetAdjacentScores();

void mapMakeBackup();

void mapRestoreBackup();

void mapCopy(Floor *srcStartFloor, Floor *srcCurrentFloor, Field *srcCurrentField, Ramp *srcStartRamp, Victim *srcStartVictim, Floor **destStartFloor, Floor **destCurrentFloor, Field **destCurrentField, Ramp **destStartRamp, Victim **destStartVictim);

void mapSender();


#endif
