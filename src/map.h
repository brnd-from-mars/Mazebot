#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include <stdlib.h>
#include "config.h"
#include "analog.h"
#include "light.h"


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

    uint8_t score;

    struct Field* next;
} Field;

typedef struct Floor {
    uint8_t id;

    struct Field* headField;
    struct Field* tailField;

    struct Field* lastVisitedField;

    struct Floor* next;
} Floor;

typedef struct Map {
    int heading : 2;
    
    struct Floor* headFloor;
    struct Floor* tailFloor;

    struct Floor* currentFloor;

    struct Field* currentField;
} Map;

struct Map mapData;
struct Map bkupMapData;


void mapInit();

void mapUpdate();

Field* mapCreateField(Point pos, bool startField);

Point mapGetAdjacentPositionGlobal(Point of, uint8_t dir);

Field* mapGetAdjacentFieldGlobal(Point of, uint8_t dir);

Field* mapFindField(Point at);

void mapSetWall(Field* field, uint8_t dir, uint8_t state);

uint8_t mapGetWall(Field* field, uint8_t dir);

void mapSender();


#endif