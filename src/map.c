#include "map.h"


void mapInit() {

    heading = NORTH;

    // create startFloor
    startFloor = malloc(sizeof(Floor));
    // set startFloor to currentFloor
    currentFloor = startFloor;

    // no next floor exists, yet
    currentFloor->next = NULL;

    // create startField and set it to currentFloor's startField
    mapCreateField(0, 0, true);
    // set startField to currentField
    currentField = currentFloor->start;

    // set whole backupDate
    bkupHeading = NORTH;
    bkupStartFloor = NULL;
    bkupCurrentFloor = NULL;
    bkupCurrentField = NULL;

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
    return (uint8_t)((dir + heading + 1)%4);
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

void mapForward() {
    // get last position
    Point lP = {.x=currentField->x, .y=currentField->y};
    // get new position
    Point nP = mapGetAdjacentPositionLocal(lP, FRONT);

    // check, if field already exists and set new currentField
    Field *nF = mapFindField(nP.x, nP.y);
    if(nF != NULL)
        currentField = nF;
    // if not -> should be an error
    
    // update map to create new observed fields
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

void mapUpdate() {
    // set current position and temporary type
    Point cP = {.x=currentField->x, .y=currentField->y};
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
    if(isBlack)
        currentField->type = 2;
    else if(isSilver || (currentField == startFloor->start)) {
        currentField->type = 3;
        mapMakeBackup();
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

    AdjacentScores aScores;

    for(uint8_t dir=0; dir<4; dir++) {
        if(currentField->neighbors[dir]==NULL)
            aScores.score[dir] = -1;
        else
            aScores.score[dir] = currentField->neighbors[dir]->score;
    }

    mapSender();

    return aScores;
}

void mapMakeBackup() {
    bkupHeading = heading;
    bkupCurrentFloor = NULL;
    bkupCurrentField = NULL;
    mapCopy(startFloor, currentFloor, currentField, &bkupStartFloor, &bkupCurrentFloor, &bkupCurrentField);
}

void mapRestoreBackup() {
    serialPrintNL();
    serialPrintInt(255);
    serialPrintNL();
    heading = bkupHeading;
    currentFloor = NULL;
    currentField = NULL;
    mapCopy(bkupStartFloor, bkupCurrentFloor, bkupCurrentField, &startFloor, &currentFloor, &currentField);
    mapSender();
}

void mapCopy(Floor *srcStartFloor, Floor *srcCurrentFloor, Field *srcCurrentField, Floor **destStartFloor, Floor **destCurrentFloor, Field **destCurrentField) {

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

    // copy the whole stuff
    
    Floor *srcFloorPtr = srcStartFloor;

    destFloorPtr = malloc(sizeof(Floor));
    (*destStartFloor) = destFloorPtr;

    do {
        if(srcFloorPtr == srcCurrentFloor)
            (*destCurrentFloor) = destFloorPtr;
        
        Field *srcFieldPtr = srcFloorPtr->start;

        Field *destFieldPtr = malloc(sizeof(Field));
        destFloorPtr->start = destFieldPtr;

        do {
            if(srcFieldPtr == srcCurrentField)
                (*destCurrentField) = destFieldPtr;

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
