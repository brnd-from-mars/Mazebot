#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include <stdlib.h>
#include "config.h"
#include "analog.h"
#include "light.h"
#include "rgb.h"


#define WALL 1
#define NO_WALL 0

#define ERR_MAP_INCON 0xFF00


typedef struct Point {
    short x;
    short y;
} Point;

typedef struct Field {
    Point pos;
    uint8_t floor;

    uint8_t walls   : 4;
    uint8_t visited : 1;
    uint8_t black   : 1;
    uint8_t silver  : 1;
    uint8_t ramp    : 1;

    short score;
} Field;

typedef struct Floor {
    uint8_t id;

    struct Field* fieldArray;
    short fieldCount;

    struct Field* startField;
    struct Field* lastVisitedField;

    struct Floor* next;
} Floor;

typedef struct Map {
    unsigned int heading : 2;
    
    struct Floor* headFloor;
    struct Floor* tailFloor;

    struct Floor* currentFloor;
    struct Field* currentField;
} Map;

struct Map mapData;
struct Map bkupMapData;

struct ScoreInfo {
    bool valid;
    short adjacentScores[4];
    short max;
} lastScoreInfo;


void mapInit();

void mapUpdate();

Field* mapCreateField(Point pos);

short mapLocalToGlobalDirection(short dir);

short mapGlobalToLocalDirection(short dir);

Point mapGetAdjacentPositionGlobal(Point of, short dir);

Field* mapGetAdjacentFieldGlobal(Point of, short dir);

Field* mapFindField(Point at);

void mapSetWall(Field* field, short dir, uint8_t state);

uint8_t mapGetWall(Field* field, short dir);

void mapRotate(short amount);

void mapForward();

void mapSetBlackInFront();

bool mapOnStartField();

bool mapSetStartScores();

void mapEvaluateScores();

void mapCopy(Map* source, Map* destination);

void mapRestoreFromBackup();

void mapSender();


#endif