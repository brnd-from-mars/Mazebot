#include "map.h"


void mapInit() {

    heading = NORTH;

    startFloor = malloc(sizeof(Floor));
    currentFloor = startFloor;

    currentFloor->next = NULL;

    mapCreateField(0, 0, true);
    currentField = currentFloor->start;

    bkupStartFloor = NULL;
    bkupCurrentFloor = NULL;
    bkupCurrentField = NULL;

    mapUpdate();
}

Field* mapCreateField(int8_t x, int8_t y, bool startField) {
    Field *new = malloc(sizeof(Field));
    Point nP = {.x=x, .y=y};

    new->x = x;
    new->y = y;
    new->type = 0;
    new->score = 0;

    // init walls, because of reasons (you don't want
    // to see what happens with out it)
    for(uint8_t i=0; i<4; i++)
        new->neighbors[i] = NULL;

    bool shouldGetDeleted = false;

    if(startField) {
        currentFloor->start = new;
        currentFloor->end = new;
    } else {
        shouldGetDeleted = true;
        for(uint8_t i=0; i<4; i++) {
            Point aP = mapGetAdjacentPositionGlobal(nP, i);
            // does this field has an adjacent field?
            Field *aF = mapFindField(aP.x, aP.y);
            if(aF != NULL) {
                if(aF == currentField) {
                    // if not -> create link
                    shouldGetDeleted = false;
                    aF->neighbors[(i+2)%4] = new;
                    new->neighbors[i] = aF;
                } else {
                    if(aF->type == 0) {
                        // if not -> create link
                        shouldGetDeleted = false;
                        aF->neighbors[(i+2)%4] = new;
                        new->neighbors[i] = aF;
                    }
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
        currentFloor->end->next = new;
        currentFloor->end = new;
    }

    new->next = NULL;

    return new;
}

Point mapGetAdjacentPositionLocal(Point aP, uint8_t dir) {
    uint8_t global = (dir + heading + 1)%4;
    return mapGetAdjacentPositionGlobal(aP, global);
}

Point mapGetAdjacentPositionGlobal(Point aP, uint8_t dir) {
    // some magic formulas that calculate the change of coordinates into
    // different directions
    // do not remove or edit this formula, it let Brendan in a big struggle
    // (because he hadn't type casted the result of abs to something signed)
    Point rP = {.x=aP.x+(int8_t)(abs(dir-2))-1, .y=aP.y+(int8_t)(abs(dir-1))-1};
    return rP;
}

Field* mapFindField(int8_t x, int8_t y) {
    Field *fieldPtr = currentFloor->start;

    Field *rtn = NULL;

    do {
        if((fieldPtr->x==x) && (fieldPtr->y==y))
            rtn = fieldPtr;
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    return rtn;
}

void mapRotate(int8_t amount) {
    int8_t new = (heading+amount);
    while(new<0)
        new+=4;
    heading = new%4;
}

void mapForward() {
    Point lP = {.x=currentField->x, .y=currentField->y};
    Point nP = mapGetAdjacentPositionLocal(lP, FRONT);

    Field *nF = mapFindField(nP.x, nP.y);

    if(nF != NULL)
        currentField = nF;
    
    mapUpdate();
}

void mapFrontFieldBlack() {
    Point cP = {.x=currentField->x, .y=currentField->y};
    Point fP = mapGetAdjacentPositionLocal(cP, FRONT);

    Field *fF = mapFindField(fP.x, fP.y);

    if(fF != NULL)
        fF->type = 2;
}

void mapUpdate() {
    Point cP = {.x=currentField->x, .y=currentField->y};
    currentField->type = 1;

    uint8_t walldata = getWallData(heading);

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
