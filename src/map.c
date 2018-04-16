#include "map.h"


void mapInit() {

    mapData.heading = NORTH;
    mapData.headFloor = malloc(sizeof(Floor));
    mapData.tailFloor = mapData.headFloor;
    mapData.currentFloor = mapData.headFloor;

    mapData.currentFloor->id = 0;
    mapData.currentFloor->fieldArray = malloc(sizeof(Field) * MAP_MAX_FIELDS_PER_FLOOR);
    mapData.currentFloor->fieldCount = 0;
    mapData.currentFloor->next = NULL;

    mapCreateField((Point) {0, 0});
    mapData.currentField = mapData.currentFloor->startField;

    mapData.headRamp = NULL;
    mapData.tailRamp = NULL;

    bkupMapData.heading = NORTH;
    bkupMapData.headFloor = NULL;
    bkupMapData.tailFloor = NULL;
    bkupMapData.currentFloor = NULL;
    bkupMapData.currentField = NULL;
    bkupMapData.headRamp = NULL;
    bkupMapData.tailRamp = NULL;

    lastScoreInfo.valid = false;

    mapUpdate();
}

void mapUpdate() {

    mapData.currentField->visited = 1;

    uint8_t walls = getWallData(mapData.heading);

    for(short dir = 0; dir < 4; dir++) {

        if((bool) (walls & (1 << dir))) {
            mapSetWall(mapData.currentField, dir, WALL);
            mapSetWall(mapGetAdjacentFieldGlobal(mapData.currentField->pos, dir), (dir+2)%4, WALL);

        } else {
            Field* connectedNeighbour = mapGetAdjacentFieldGlobal(mapData.currentField->pos, dir);
            if(connectedNeighbour == NULL) {

                Point pos = mapGetAdjacentPositionGlobal(mapData.currentField->pos, dir);
                connectedNeighbour = mapCreateField(pos);
            }
            mapSetWall(mapData.currentField, dir, NO_WALL);
            mapSetWall(connectedNeighbour, (dir+2)%4, NO_WALL);
        }
    }

    if(isSilver ||
      mapData.currentField->silver == 1 ||
      mapData.currentField == mapData.currentFloor->startField) {
        mapData.currentField->silver = 1;
        mapCopy(&mapData, &bkupMapData);
    }

    mapSender();
    lastScoreInfo.valid = false;
}

Field* mapCreateField(Point pos) {

    short index = mapData.currentFloor->fieldCount;
    bool startField = (index == 0);

    mapData.currentFloor->fieldCount++;

    Field* fieldPtr   = &mapData.currentFloor->fieldArray[index];

    fieldPtr->pos     = pos;
    fieldPtr->floor   = mapData.currentFloor->id;
    fieldPtr->walls   = 0B1111;
    fieldPtr->visited = 0;
    fieldPtr->black   = 0;
    fieldPtr->silver  = 0;
    fieldPtr->ramp    = 0;
    fieldPtr->score   = 0;

    bool shouldGetDeleted = false;

    if(startField) {

        mapData.currentFloor->lastVisitedField = fieldPtr;
        mapData.currentFloor->startField = fieldPtr;

    } else {

        shouldGetDeleted = true;

        for(short dir = 0; dir < 4; dir++) {
            
            Field* neighbour = mapGetAdjacentFieldGlobal(pos, dir);
            if(neighbour != NULL) {
                
                shouldGetDeleted = false;
                mapSetWall(fieldPtr, dir, NO_WALL);
                mapSetWall(neighbour, (dir+2)%4, NO_WALL);

            }
        }

        if(shouldGetDeleted) {
            mapData.currentFloor->fieldCount--;
            return NULL;
        }
    }

    return fieldPtr;
}

Floor* mapCreateFloor() {

    mapData.tailFloor->next = malloc(sizeof(Floor));
    mapData.tailFloor->next->id = mapData.tailFloor->id + 1;
    mapData.tailFloor->next->fieldArray = malloc(sizeof(Field) * MAP_MAX_FIELDS_PER_FLOOR);
    mapData.tailFloor->next->fieldCount = 0;
    mapData.tailFloor->next->next = NULL;
    
    mapData.tailFloor = mapData.tailFloor->next;
    mapData.currentFloor = mapData.tailFloor;

    mapCreateField((Point) {0, 0});

    return mapData.tailFloor;
}

Ramp* mapCreateRamp() {

    uint8_t fl1 = mapData.currentFloor->id;
    Point fd1 = mapData.currentField->pos;

    mapCreateFloor();

    if(mapData.headRamp == NULL) {
        mapData.headRamp = malloc(sizeof(Ramp));
        mapData.tailRamp = mapData.headRamp;
    } else {
        mapData.tailRamp->next = malloc(sizeof(Ramp));
        mapData.tailRamp = mapData.tailRamp->next;
    }

    mapData.tailRamp->floor1 = fl1;
    mapData.tailRamp->floor2 = mapData.currentFloor->id;
    mapData.tailRamp->field1 = fd1;
    mapData.tailRamp->field2 = mapData.currentField->pos;
    mapData.tailRamp->next = NULL;

    return mapData.tailRamp;
}

short mapLocalToGlobalDirection(short loc) {

    short glob = (loc + mapData.heading + 1) % 4;
    return ((glob < 0) ? (glob + 4) : glob);
}

short mapGlobalToLocalDirection(short glob) {

    short loc = (glob - mapData.heading - 1) % 4;
    return ((loc < 0) ? (loc + 4) : loc);
}

Point mapGetAdjacentPositionGlobal(Point of, short dir) {

    // plot it for 0<=dir<=3 and you will get the idea
    Point rP = {of.x+(abs(dir-2))-1, of.y+(abs(dir-1))-1};
    return rP;
}

Field* mapGetAdjacentFieldGlobal(Point of, short dir) {

    Point rP = mapGetAdjacentPositionGlobal(of, dir);
    return mapFindField(rP);
}

Field* mapFindField(Point at) {

    Field* fieldPtr = NULL;

    for(short i = 0; i < mapData.currentFloor->fieldCount; i++) {
    
        fieldPtr = &mapData.currentFloor->fieldArray[i];
        if(fieldPtr->pos.x == at.x && fieldPtr->pos.y == at.y)
            return fieldPtr;

    }

    return NULL;
}

Floor* mapFindFloor(uint8_t id) {

    Floor* floorPtr = mapData.headFloor;

    while(floorPtr != NULL) {

        if(floorPtr->id == id)
            return floorPtr;

        floorPtr = floorPtr->next;
    }

    return NULL;
}

Ramp* mapFindRamp(Point end) {

    Ramp* rampPtr = mapData.headRamp;

    while(rampPtr != NULL) {

        if((rampPtr->field1.x == end.x) && (rampPtr->field1.y == end.y))
            return rampPtr;
        if((rampPtr->field2.x == end.x) && (rampPtr->field2.y == end.y))
            return rampPtr;
        
        rampPtr = rampPtr->next;
    }

    return NULL;
}

void mapSetWall(Field* field, short dir, uint8_t state) {

    if(field == NULL)
        return;

    field->walls &= ~(1 << dir);
    if(state == WALL)
        field->walls |= (1 << dir);
}

uint8_t mapGetWall(Field* field, short dir) {

    return (((bool) (field->walls & (1 << dir))) ? WALL : NO_WALL);
}

void mapRotate(short amount) {

    mapData.heading += amount;
    mapData.heading = mapData.heading % 4;

    if(mapData.heading < 0)
        mapData.heading += 4;

    mapUpdate();
}

void mapForward(bool update) {

    Field* front = mapGetAdjacentFieldGlobal(mapData.currentField->pos, mapData.heading);

    mapData.currentField = front;
    mapData.currentFloor->lastVisitedField = mapData.currentField;

    if(update) {
        mapUpdate();
    }
}

void mapSetBlackInFront() {

    Field* front = mapGetAdjacentFieldGlobal(mapData.currentField->pos, mapData.heading);

    front->visited = 1;
    front->black = 1;
    front->score = -1;
    
    mapUpdate();
}

void mapChangeFloor(Ramp* ramp) {

    uint8_t newID = ramp->floor1;
    if(newID == mapData.currentFloor->id) {
        newID = ramp->floor2;
    }

    mapData.currentFloor = mapFindFloor(newID);
    mapData.currentField = mapData.currentFloor->lastVisitedField;
}

void mapSetRamp() {

    mapForward(false);

    Ramp* currentRamp = NULL;

    if(mapData.currentField->ramp) {

        currentRamp = mapFindRamp(mapData.currentField->pos);

        if(currentRamp != NULL) {
            mapChangeFloor(currentRamp);
        } else { // should never happen
            currentRamp = mapCreateRamp();
        }

    } else {
        mapData.currentField->ramp = 1;
        currentRamp = mapCreateRamp();
    }

    mapData.currentField->ramp = 1;

    // map is now pointing to final floor to the last tile on the ramp
}

void mapFinishRamp() {

    mapForward(true);
    mapSender();
}

bool mapOnStartField() {

    return (mapData.currentField == mapData.currentFloor->startField);
}

bool mapSetStartScores() {

    Field* fieldPtr = NULL;
    bool backToOrigin = true;

    for(short i = 0; i < mapData.currentFloor->fieldCount; i++) {

        fieldPtr = &mapData.currentFloor->fieldArray[i];

        if(fieldPtr->black)
            fieldPtr->score = -1;
        else if(fieldPtr->visited)
            fieldPtr->score = 0;
        else {
            fieldPtr->score = 255;
            backToOrigin = false;
        }

    }

    return backToOrigin;
}

void mapEvaluateScores() {

    Field* fieldPtr = NULL;
    bool backToOrigin = mapSetStartScores();
    
    if(backToOrigin && !mapOnStartField()) {
        mapData.currentFloor->startField->visited = 0;
        mapSetStartScores();
    }

    if(!mapOnStartField() || (mapOnStartField() && !backToOrigin)) {

        short currentScore = 255;

        while(mapData.currentField->score == 0) {

            fieldPtr = NULL;

            for(short i = 0; i < mapData.currentFloor->fieldCount; i++) {

                fieldPtr = &mapData.currentFloor->fieldArray[i];

                if(fieldPtr->score == 0) {
                    for(short dir = 0; dir < 4; dir++) {
                        if(mapGetWall(fieldPtr, dir) == NO_WALL) {
                            Field* neighbour = mapGetAdjacentFieldGlobal(fieldPtr->pos, dir);
                            if(neighbour != NULL && neighbour->score == currentScore) {
                                fieldPtr->score = currentScore - 1;
                            }
                        }
                    }
                }

            }

            currentScore--;
        }
    }

    lastScoreInfo.max = -256;

    for(short dir = 0; dir < 4; dir++) {

        fieldPtr = mapGetAdjacentFieldGlobal(mapData.currentField->pos, dir);
        int local = mapGlobalToLocalDirection(dir);

        if((mapGetWall(mapData.currentField, dir) == WALL) || (fieldPtr == NULL)) {
            lastScoreInfo.adjacentScores[local] = -1;
        } else {
            lastScoreInfo.adjacentScores[local] = fieldPtr->score;
        }
            
        if(lastScoreInfo.adjacentScores[local] > lastScoreInfo.max)
            lastScoreInfo.max = lastScoreInfo.adjacentScores[local];

    }

    lastScoreInfo.valid = true;
}

void mapCopy(Map* source, Map* destination) {

    rgbBlink(64, 52, 130, 0, 300);

    destination->heading = source->heading;

    Floor* destinationFloorPtr = destination->headFloor;
    Floor* sourceFloorPtr = source->headFloor;

    while(destinationFloorPtr != NULL) {

        Floor* next = destinationFloorPtr->next;
        free(destinationFloorPtr->fieldArray);
        free(destinationFloorPtr);
        destinationFloorPtr = next;
    }

    while(sourceFloorPtr != NULL) {

        if(destinationFloorPtr == NULL)
            destinationFloorPtr = malloc(sizeof(Floor));
        else {
            destinationFloorPtr->next = malloc(sizeof(Floor));
            destinationFloorPtr = destinationFloorPtr->next;
        }

        destinationFloorPtr->id         = sourceFloorPtr->id;
        destinationFloorPtr->fieldArray = malloc(sizeof(Field) * MAP_MAX_FIELDS_PER_FLOOR);
        destinationFloorPtr->fieldCount = sourceFloorPtr->fieldCount;
        destinationFloorPtr->next       = NULL;

        Field* destinationFieldPtr = NULL;
        Field* sourceFieldPtr      = NULL;

        for(short i = 0; i < destinationFloorPtr->fieldCount; i++) {

            destinationFieldPtr = &destinationFloorPtr->fieldArray[i];
            sourceFieldPtr      = &sourceFloorPtr->fieldArray[i];

            destinationFieldPtr->pos     = sourceFieldPtr->pos;
            destinationFieldPtr->floor   = sourceFieldPtr->floor;
            destinationFieldPtr->walls   = sourceFieldPtr->walls;
            destinationFieldPtr->visited = sourceFieldPtr->visited;
            destinationFieldPtr->black   = sourceFieldPtr->black;
            destinationFieldPtr->silver  = sourceFieldPtr->silver;
            destinationFieldPtr->ramp    = sourceFieldPtr->ramp;
            destinationFieldPtr->score   = sourceFieldPtr->score;

            if(sourceFloorPtr->startField == sourceFieldPtr)
                destinationFloorPtr->startField = destinationFieldPtr;
            if(sourceFloorPtr->lastVisitedField == destinationFieldPtr)
                destinationFloorPtr->lastVisitedField = destinationFieldPtr;
            if(source->currentField == sourceFieldPtr)
                destination->currentField = destinationFieldPtr;
        }

        if(source->headFloor == sourceFloorPtr)
            destination->headFloor = destinationFloorPtr;
        if(source->tailFloor == sourceFloorPtr);
            destination->tailFloor = destinationFloorPtr;
        if(source->currentFloor == sourceFloorPtr)
            destination->currentFloor = destinationFloorPtr;

        sourceFloorPtr = sourceFloorPtr->next;
    }

    Ramp* destinationRampPtr = destination->headRamp;
    Ramp* sourceRampPtr = source->headRamp;

    while(destinationRampPtr != NULL) {

        Ramp* next = destinationRampPtr->next;
        free(destinationRampPtr);
        destinationRampPtr = next;
    }

    while(sourceRampPtr != NULL) {

        if(destinationRampPtr == NULL)
            destinationRampPtr = malloc(sizeof(Ramp));
        else {
            destinationRampPtr->next = malloc(sizeof(Ramp));
            destinationRampPtr = destinationRampPtr->next;
        }

        destinationRampPtr->floor1 = sourceRampPtr->floor1;
        destinationRampPtr->floor2 = sourceRampPtr->floor2;
        destinationRampPtr->field1 = sourceRampPtr->field1;
        destinationRampPtr->field2 = sourceRampPtr->field2;
    }
}

void mapRestoreFromBackup() {

    mapCopy(&bkupMapData, &mapData);
    mapSender();
}

void mapSender() {

    Field* fieldPtr = NULL;


    serialPrintNL();
    serialPrintInt(bkupMapData.currentField->pos.x);
    serialPrintInt(bkupMapData.currentField->pos.y);
    serialPrintInt(bkupMapData.heading);
    serialPrintNL();

    for(short i = 0; i < bkupMapData.currentFloor->fieldCount; i++) {

        fieldPtr = &bkupMapData.currentFloor->fieldArray[i];

        serialPrintInt(fieldPtr->pos.x);
        serialPrintInt(fieldPtr->pos.y);

        if(fieldPtr->ramp)
            serialPrintInt(4);
        else if(fieldPtr->silver)
            serialPrintInt(3);
        else if(fieldPtr->black)
            serialPrintInt(2);
        else if(fieldPtr->visited)
            serialPrintInt(1);
        else
            serialPrintInt(0);

        serialPrintInt(fieldPtr->score);

        for(int dir = 0; dir < 4; dir++)
            serialPrintInt(mapGetWall(fieldPtr, dir));

        serialPrintNL();
    }

    serialPrintNL();
}
