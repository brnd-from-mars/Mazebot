#include "map.h"


void mapInit() {

    mapData.heading = NORTH;
    mapData.headFloor = malloc(sizeof(Floor));
    mapData.tailFloor = mapData.headFloor;
    mapData.currentFloor = mapData.headFloor;
    mapData.currentFloor->id = 0;
    mapData.currentFloor->next = NULL;

    mapCreateField((Point) {0, 0}, true);
    mapData.currentField = mapData.currentFloor->headField;
    mapData.currentFloor->tailField = mapData.currentField;

    bkupMapData.heading = NORTH;
    bkupMapData.headFloor = NULL;
    bkupMapData.tailFloor = NULL;
    bkupMapData.currentFloor = NULL;
    bkupMapData.currentField = NULL;

    mapUpdate();
}

void mapUpdate() {

    mapData.currentField->visited = 1;

    uint8_t walls = getWallData(mapData.heading);

    for(uint8_t dir = 0; dir < 4; dir++) {

        if((bool) (walls & (1 << dir))) {
            mapSetWall(mapData.currentField, dir, WALL);
            mapSetWall(mapGetAdjacentFieldGlobal(mapData.currentField->pos, dir), (dir+2)%4, WALL);

        } else {
            Field* connectedNeighbour = mapGetAdjacentFieldGlobal(mapData.currentField->pos, dir);
            if(connectedNeighbour == NULL) {

                Point pos = mapGetAdjacentPositionGlobal(mapData.currentField->pos, dir);
                connectedNeighbour = mapCreateField(pos, false);
            }
            mapSetWall(mapData.currentField, dir, NO_WALL);
            mapSetWall(connectedNeighbour, (dir+2)%4, NO_WALL);
        }
    }
}

Field* mapCreateField(Point pos, bool startField) {

    Field* new   = malloc(sizeof(Field));
    new->pos     = pos;
    new->floor   = mapData.currentFloor->id;
    new->walls   = 0B1111;
    new->visited = 0;
    new->black   = 0;
    new->silver  = 0;
    new->ramp    = 0;
    new->score   = 0;

    bool shouldGetDeleted = false;

    if(startField) {

        mapData.currentFloor->headField = new;
        mapData.currentFloor->tailField = new;
        mapData.currentFloor->lastVisitedField = new;

    } else {

        shouldGetDeleted = true;

        for(uint8_t dir = 0; dir < 4; dir++) {
            
            Field* neighbour = mapGetAdjacentFieldGlobal(pos, dir);
            if(neighbour != NULL) {
                
                shouldGetDeleted = false;
                mapSetWall(new, dir, NO_WALL);
                mapSetWall(neighbour, (dir+2)%4, NO_WALL);

            }
        }

        if(shouldGetDeleted) {
            free(new);
            return NULL;
        }

        mapData.currentFloor->tailField->next = new;
        mapData.currentFloor->tailField = new;
    }

    new->next = NULL;

    return new;
}

Point mapGetAdjacentPositionGlobal(Point of, uint8_t dir) {

    Point rP = {.x=of.x+(int8_t)(abs(dir-2))-1, .y=of.y+(int8_t)(abs(dir-1))-1};
    return rP;
}

Field* mapGetAdjacentFieldGlobal(Point of, uint8_t dir) {

    Point rP = mapGetAdjacentPositionGlobal(of, dir);
    return mapFindField(rP);
}

Field* mapFindField(Point at) {

    Field* fieldPtr = mapData.currentFloor->headField;

    while(fieldPtr != NULL) {

        if(fieldPtr->pos.x == at.x && fieldPtr->pos.y == at.y)
            return fieldPtr;
        fieldPtr = fieldPtr->next;
    }

    return NULL;
}

void mapSetWall(Field* field, uint8_t dir, uint8_t state) {

    if(field == NULL)
        return;

    field->walls &= ~(1 << dir);
    if(state == WALL)
        field->walls |= (1 << dir);
}

uint8_t mapGetWall(Field* field, uint8_t dir) {

    return (((bool) (field->walls & (1 << dir))) ? WALL : NO_WALL);
}

void mapSender() {

    Field* fieldPtr = mapData.currentField;

    while(fieldPtr != NULL) {
        serialPrintInt(fieldPtr->pos.x);
        serialPrintInt(fieldPtr->pos.y);
        serialPrintInt(fieldPtr->walls);
        serialPrintNL();
        fieldPtr = fieldPtr->next;
    }

    serialPrintNL();
}