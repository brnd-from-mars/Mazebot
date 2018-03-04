#include "map.h"


void mapInit() {

    heading = NORTH;

    // create startFloor
    startFloor = malloc(sizeof(Floor));
    // set startFloor to currentFloor
    currentFloor = startFloor;
    currentFloor->id = 0;
    currentFloor->finished = false;

    // no next floor exists, yet
    currentFloor->next = NULL;

    // create startField and set it to currentFloor's startField
    mapCreateField(0, 0, true);
    // set startField to currentField
    currentField = currentFloor->start;
    currentFloor->lastField = currentField;

    firstRamp = NULL;

    // set whole backupDate
    bkupHeading = NORTH;
    bkupStartFloor = NULL;
    bkupCurrentFloor = NULL;
    bkupCurrentField = NULL;
    bkupFirstRamp = NULL;

    // update map
    // -> create surrounding visible fields
    // -> link them to already known fields
    // -> make first backup
    mapUpdate();
}

Field* mapCreateField(int8_t x, int8_t y, bool startField) {
    Field *new = malloc(sizeof(Field));
    Point nP = {.x=x, .y=y};

    new->x = x;
    new->y = y;
    new->type = 0;
    new->score = 0;

    // init walls, other functions depend on that
    // the pointers to neighbor walls in a struct should be
    // either NULL or direct to another field
    // -> nether unititialized
    for(uint8_t i=0; i<4; i++)
        new->neighbors[i] = NULL;

    // explanation: look down in this function
    bool shouldGetDeleted = false;

    // if field is startField
    if(startField) {
        // set head and tail of current floor linked list
        currentFloor->start = new;
        currentFloor->end = new;
    // if not
    } else {
        // field should get deleted as long as it don't have
        shouldGetDeleted = true;
        for(uint8_t i=0; i<4; i++) {
            Point aP = mapGetAdjacentPositionGlobal(nP, i);
            // ... any known adjacent fields
            Field *aF = mapFindField(aP.x, aP.y);
            if(aF != NULL) {
                if(aF == currentField) {
                    // if not -> create link
                    shouldGetDeleted = false;
                    aF->neighbors[(i+2)%4] = new;
                    new->neighbors[i] = aF;
                } else {
                    // if adjacent field not visited ...
                    if(aF->type == 0) {
                        // ... create link
                        shouldGetDeleted = false;
                        aF->neighbors[(i+2)%4] = new;
                        new->neighbors[i] = aF;
                    }
                    // otherwise the field to create should have
                    // been already created
                    // -> implement an error catching at this point?
                }
            }
        }
    }

    // only fields that are adjacent to an other can be created
    // otherwise it would end up without any reference to it
    // exception: floor-startfields have a pointer that reference them
    // in the floor struct that belongs to it
    if(shouldGetDeleted) {
        free(new);
        return NULL;
    } else if(!startField) {
        // set new tail of field linked list for current floor
        currentFloor->end->next = new;
        currentFloor->end = new;
    }

    new->next = NULL;

    return new;
}

uint8_t mapLocalToGlobalDirection(uint8_t local) {
    return (uint8_t)((local + heading + 1)%4);
}

Point mapGetAdjacentPositionLocal(Point aP, uint8_t dir) {
    uint8_t global = mapLocalToGlobalDirection(dir);
    return mapGetAdjacentPositionGlobal(aP, global);
}

Point mapGetAdjacentPositionGlobal(Point aP, uint8_t dir) {
    // some magic formulas that calculate the change of coordinates into
    // different directions
    // do not remove or edit this formula, it let Brendan in a big struggle
    // for a long time (because he hadn't type casted the result of
    // abs to something signed)
    Point rP = {.x=aP.x+(int8_t)(abs(dir-2))-1, .y=aP.y+(int8_t)(abs(dir-1))-1};
    return rP;
}

Field* mapFindField(int8_t x, int8_t y) {
    // let cursor point to current floor's startField
    Field *fieldPtr = currentFloor->start;

    // iterate through all fields in current floor
    do {
        // if coordinates match requested ones
        if((fieldPtr->x==x) && (fieldPtr->y==y))
            // return current cursor
            return fieldPtr;
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    return NULL;
}

void mapRotate(int8_t amount) {
    // this function does not rotate the map!
    // it just keeps track of the robot's current
    // orientation
    int8_t new = (heading+amount);
    while(new<0)
        new+=4;
    heading = new%4;
}

void mapForward(bool ramp) {
    // get last position
    Point lP = {.x=currentField->x, .y=currentField->y};
    // get new position
    Point nP = mapGetAdjacentPositionLocal(lP, FRONT);

    // check, if field already exists and set new currentField
    Field *nF = mapFindField(nP.x, nP.y);
    if(nF != NULL)
        currentField = nF;
    // if not -> should be an error

    currentFloor->lastField = currentField;
    
    // update map to create new observed fields
    if(!ramp)
        mapUpdate();
}

void mapFrontFieldBlack() {
    // get current position
    Point cP = {.x=currentField->x, .y=currentField->y};
    // get position in front of the robot
    Point fP = mapGetAdjacentPositionLocal(cP, FRONT);

    // check, if field already exists and set type to black
    Field *fF = mapFindField(fP.x, fP.y);
    if(fF != NULL)
        fF->type = 2;
    // if not -> should be an error
}

void mapSetRamp() {
    mapForward(true);
    currentField->type = 4;

    bool newRamp = true;
    Ramp *rampPtr = firstRamp;
    Ramp *lastRampPtr = NULL;
    while(rampPtr!=NULL && newRamp) {
        Point rP = {.x=currentField->x, .y=currentField->y};
        if(rampPtr->floor1==currentFloor->id && rampPtr->field1.x==rP.x && rampPtr->field1.y==rP.y)
            newRamp = false;
        if(rampPtr->floor2==currentFloor->id && rampPtr->field2.x==rP.x && rampPtr->field2.y==rP.y)
            newRamp = false;

        lastRampPtr = rampPtr;
        rampPtr = rampPtr->next;
    }

    if(newRamp) {
        if(lastRampPtr == NULL) {
            firstRamp = malloc(sizeof(Ramp));
            rampPtr = firstRamp;
        } else {
            lastRampPtr->next = malloc(sizeof(Ramp));
            rampPtr = lastRampPtr->next;
        }

        Point cP = {.x=currentField->x, .y=currentField->y};

        rampPtr->next = NULL;
        rampPtr->floor1 = currentFloor->id;
        rampPtr->field1 = cP;

        currentFloor->next = malloc(sizeof(Floor));
        currentFloor->next->id = currentFloor->id+1;
        currentFloor = currentFloor->next;
        currentFloor->next = NULL;
        currentFloor->finished = false;

        mapCreateField(0, 0, true);
        currentField = currentFloor->start;
        currentFloor->lastField = currentField;

        currentField->type = 4;

        cP.x = currentField->x;
        cP.y = currentField->y;

        rampPtr->floor2 = currentFloor->id;
        rampPtr->field2 = cP;

        Point nP = mapGetAdjacentPositionLocal(cP, FRONT);
        mapCreateField(nP.x, nP.y, false);

        mapForward(true);
    } else {
        if(lastRampPtr->floor1 == currentFloor->id) {
            Floor *floorPtr = startFloor;
            while(floorPtr!=NULL && floorPtr->id!=lastRampPtr->floor2)
                floorPtr = floorPtr->next;
            currentFloor = floorPtr;
            currentField = currentFloor->lastField;
            mapForward(true);
        } else if(lastRampPtr->floor2 == currentFloor->id) {
            Floor *floorPtr = startFloor;
            while(floorPtr!=NULL && floorPtr->id!=lastRampPtr->floor1)
                floorPtr = floorPtr->next;
            currentFloor = floorPtr;
            currentField = currentFloor->lastField;
            mapForward(true);
        }
    }
}

void mapFinishRamp() {
    serialPrintInt(255);
    serialPrintNL();
    mapSender();
}

void mapUpdate() {
    // set current position and temporary type
    Point cP = {.x=currentField->x, .y=currentField->y};
    if(currentField->type != 4)
        currentField->type = 1;

    // request information about surrounding fields
    uint8_t walldata = getWallData(heading);

    // iterate through all directions
    for(uint8_t dir=0; dir<4; dir++) {
        // if wall detected
        if((bool)(walldata & (1<<dir))) {
            // but next field linked to current
            if(currentField->neighbors[dir]!=NULL) {
                // then remove link between them
                currentField->neighbors[dir]->neighbors[(dir+2)%4] = NULL;
                currentField->neighbors[dir] = NULL;
            }
        }
        // if no wall detected
        else {
            // but no link to another field
            if(currentField->neighbors[dir]==NULL) {
                Point aP = mapGetAdjacentPositionGlobal(cP, dir);
                Field *aF = mapFindField(aP.x, aP.y);
                // but field exists
                if(aF!=NULL) {
                    // wild ERROR appeared
                    // -> map corrupted
                    // link them anyway
                    currentField->neighbors[dir] = aF;
                    aF->neighbors[(dir+2)%4] = currentField;
                } else {
                    // then create and link field
                    mapCreateField(aP.x, aP.y, false);
                }
            }
        }
    }

    // specify field type and if necessary store backup
    if(currentField->type != 4) {
        if(isBlack)
            currentField->type = 2;
        else if(isSilver || (currentField == startFloor->start)) {
            currentField->type = 3;
            mapMakeBackup();
        }
    }
}

AdjacentScores mapGetAdjacentScores() {
    Field *fieldPtr = currentFloor->start;

    bool flag = false;

    do {
        if(fieldPtr->type == 2)
            fieldPtr->score = -1;
        else if(fieldPtr->type != 0)
            fieldPtr->score = 0;
        else {
            fieldPtr->score = 127;
            flag = true;
        }
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    if(flag==false && currentField!=currentFloor->start) {
        fieldPtr = currentFloor->start;
        currentFloor->start->type = 0;
        do {
            if(fieldPtr->type == 2)
                fieldPtr->score = -1;
            else if(fieldPtr->type != 0)
                fieldPtr->score = 0;
            else
                fieldPtr->score = 127;
            fieldPtr = fieldPtr->next;
        } while(fieldPtr != NULL);
    }

    if((currentField!=currentFloor->start) || (currentField==currentFloor->start && flag==true)) {

        int8_t i=127;

        while(currentField->score==0) {
            fieldPtr = currentFloor->start;
            do {
                if(fieldPtr->score == 0) {
                    for(uint8_t dir=0; dir<4; dir++) {
                        if(fieldPtr->neighbors[dir]!=NULL) {
                            if(fieldPtr->neighbors[dir]->score == i) {
                                fieldPtr->score = i-1;
                            }
                        }
                    }
                }
                fieldPtr = fieldPtr->next;
            } while(fieldPtr != NULL);
            i--;
        }
    
    }

    bool finished=true;

    AdjacentScores aScores;

    for(uint8_t dir=0; dir<4; dir++) {
        if(currentField->neighbors[dir]==NULL)
            aScores.score[dir] = -1;
        else
            aScores.score[dir] = currentField->neighbors[dir]->score;

        if(aScores.score[dir]>0) {
            finished = false;
        }
    }

    if(finished) {
        currentFloor->finished = true;
        rgbSet(255, 255, 255, 0);
        while(true);
    }

    mapSender();

    return aScores;
}

void mapMakeBackup() {
    bkupHeading = heading;
    bkupCurrentFloor = NULL;
    bkupCurrentField = NULL;
    mapCopy(startFloor, currentFloor, currentField, firstRamp, &bkupStartFloor, &bkupCurrentFloor, &bkupCurrentField, &bkupFirstRamp);
}

void mapRestoreBackup() {
    serialPrintNL();
    serialPrintInt(255);
    serialPrintNL();
    heading = bkupHeading;
    currentFloor = NULL;
    currentField = NULL;
    mapCopy(bkupStartFloor, bkupCurrentFloor, bkupCurrentField, bkupFirstRamp, &startFloor, &currentFloor, &currentField, &firstRamp);
    mapSender();
}

void mapCopy(Floor *srcStartFloor, Floor *srcCurrentFloor, Field *srcCurrentField, Ramp *srcStartRamp, Floor **destStartFloor, Floor **destCurrentFloor, Field **destCurrentField, Ramp **destStartRamp) {

    // delete old stuff at the source

    Floor *destFloorPtr = (*destStartFloor);

    // iterate through all floors
    while(destFloorPtr!=NULL) {
        Field *destFieldPtr = destFloorPtr->start;
        // iterate through all fields
        do {
            Field *nextField = destFieldPtr->next;
            // delete field
            free(destFieldPtr);
            destFieldPtr = nextField;
        } while(destFieldPtr != NULL);
        Floor *nextFloor = destFloorPtr->next;
        // delete floor
        free(destFloorPtr);
        destFloorPtr = nextFloor;
    }

    Ramp *destRampPtr = (*destStartRamp);

    while(destRampPtr!=NULL) {
        Ramp *nextRamp = destRampPtr->next;
        free(destRampPtr);
        destRampPtr = nextRamp;
    }

    // copy the whole stuff

    Ramp *srcRampPtr = srcRampPtr;
    destRampPtr = NULL;
    (*destStartRamp) = destRampPtr;


    while(srcRampPtr!=NULL) {
        if(destRampPtr==NULL) {
            destRampPtr = malloc(sizeof(Ramp));
            (*destStartRamp) = destRampPtr;
        } else {
            destRampPtr->next = malloc(sizeof(Ramp));
            destRampPtr = destRampPtr->next;
        }

        destRampPtr->floor1 = srcRampPtr->floor1;
        destRampPtr->field1 = srcRampPtr->field1;
        destRampPtr->floor2 = srcRampPtr->floor2;
        destRampPtr->field2 = srcRampPtr->field2;
        destRampPtr->next = NULL;

        srcRampPtr = srcRampPtr->next;
    }
    
    Floor *srcFloorPtr = srcStartFloor;

    destFloorPtr = malloc(sizeof(Floor));
    (*destStartFloor) = destFloorPtr;

    do {
        if(srcFloorPtr == srcCurrentFloor)
            (*destCurrentFloor) = destFloorPtr;

        destFloorPtr->finished = srcFloorPtr->finished;
        destFloorPtr->id = srcFloorPtr->id;
        
        Field *srcFieldPtr = srcFloorPtr->start;

        Field *destFieldPtr = malloc(sizeof(Field));
        destFloorPtr->start = destFieldPtr;

        do {
            if(srcFieldPtr == srcCurrentField)
                (*destCurrentField) = destFieldPtr;

            if(srcFieldPtr == srcCurrentFloor->lastField)
                (*destCurrentFloor)->lastField = destFieldPtr;

            destFieldPtr->x = srcFieldPtr->x;
            destFieldPtr->y = srcFieldPtr->y;
            destFieldPtr->type = srcFieldPtr->type;
            destFieldPtr->score = 0;
            destFieldPtr->neighbors[0] = NULL;
            destFieldPtr->neighbors[1] = NULL;
            destFieldPtr->neighbors[2] = NULL;
            destFieldPtr->neighbors[3] = NULL;

            if(srcFieldPtr->next != NULL) {
                destFieldPtr->next = malloc(sizeof(Field));
                destFieldPtr = destFieldPtr->next;
            } else {
                destFieldPtr->next = NULL;
                destFloorPtr->end = destFieldPtr;
            }

            srcFieldPtr = srcFieldPtr->next;
        } while(srcFieldPtr != NULL);

        srcFieldPtr = srcFloorPtr->start;
        destFieldPtr = destFloorPtr->start;

        do {
            for(uint8_t dir=0; dir<4; dir++) {
                if((srcFieldPtr->neighbors[dir]!=NULL) && (destFieldPtr->neighbors[dir]==NULL)) {
                    Point targetPoint = {.x=srcFieldPtr->neighbors[dir]->x, .y=srcFieldPtr->neighbors[dir]->y};
                    Field *targetDestField = destFieldPtr->next;
                    while((targetDestField!=NULL) && !((targetDestField->x==targetPoint.x) && (targetDestField->y==targetPoint.y))) {
                        targetDestField = targetDestField->next;
                    }
                    if(targetDestField!=NULL) {
                        destFieldPtr->neighbors[dir]=targetDestField;
                        targetDestField->neighbors[(dir+2)%4] = destFieldPtr;
                    }
                }
            }
            srcFieldPtr = srcFieldPtr->next;
            destFieldPtr = destFieldPtr->next;
        } while((srcFieldPtr!=NULL) && (destFieldPtr!=NULL));

        if(srcFloorPtr->next != NULL) {
            destFloorPtr->next = malloc(sizeof(Floor));
            destFloorPtr = destFloorPtr->next;
        } else
            destFloorPtr->next = NULL;

        srcFloorPtr = srcFloorPtr->next;
    } while(srcFloorPtr != NULL);

}

void mapSender() {
    Field *fieldPtr = currentFloor->start;

    serialPrintNL();
    serialPrintNL();
    serialPrintInt(currentField->x);
    serialPrintInt(currentField->y);
    serialPrintInt(heading);
    serialPrintNL();

    do {
        serialPrintInt(fieldPtr->x);
        serialPrintInt(fieldPtr->y);
        serialPrintInt(fieldPtr->type);
        serialPrintInt(fieldPtr->score);
        for(uint8_t dir=0; dir<4; dir++)
            serialPrintInt(((fieldPtr->neighbors[dir]==NULL)?1:0));
        serialPrintNL();
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    serialPrintNL();
}
