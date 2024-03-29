#ifndef MAP_H_
#define MAP_H_


#include <Arduino.h>
#include <stdlib.h>
#include "config.h"
#include "analog.h"
#include "light.h"
#include "rgb.h"
#include "driveStateMachine.h"
#include "ramp.h"


#define WALL 1
#define NO_WALL 0

#define ERR_MAP_INCON 0xF00


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

typedef struct Ramp {
    uint8_t floor1;
    uint8_t floor2;

    struct Point field1;
    struct Point field2;

    struct Ramp* next;
} Ramp;

typedef struct Victim {
    uint8_t floor;
    struct Point field;
    unsigned int direction : 2;

    unsigned int type : 2;

    struct Victim* next;
} Victim;

typedef struct Map {
    unsigned int heading : 2;
    
    struct Floor* headFloor;
    struct Floor* tailFloor;

    struct Floor* currentFloor;
    struct Field* currentField;

    struct Ramp* headRamp;
    struct Ramp* tailRamp;

    struct Victim* headVictim;
    struct Victim* tailVictim;
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

Floor* mapCreateFloor();

Ramp* mapCreateRamp();

Victim* mapCreateVictim(short localDir, uint8_t type);

short mapLocalToGlobalDirection(short dir);

short mapGlobalToLocalDirection(short dir);

Point mapGetAdjacentPositionGlobal(Point of, short dir);

Field* mapGetAdjacentFieldGlobal(Point of, short dir);

Field* mapFindField(Point at);

Floor* mapFindFloor(uint8_t id);

Ramp* mapFindRamp(Point end);

void mapSetWall(Field* field, short dir, uint8_t state);

uint8_t mapGetWall(Field* field, short dir);

bool mapRotate(short amount, bool verify);

void mapForward(bool update);

void mapSetBlackInFront();

void mapChangeFloor(Ramp* ramp);

void mapSetRamp();

void mapFinishRamp();

int8_t mapGetVictimType(short dir);

bool mapOnStartField();

bool mapSetStartScores();

void mapEvaluateScores();

void mapCopy(Map* source, Map* destination);

void mapRestoreFromBackup();

void mapSender();

void mapDelete(Map* map);

void mapReset();


#endif