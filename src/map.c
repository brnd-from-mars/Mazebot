#include "map.h"


void mapInit() {heading = NORTH;

    currentFloor = malloc(sizeof(struct Floor));

    mapCreateField(0, 0, true);
    currentField = currentFloor->start;

    mapUpdate();
}

Field* mapCreateField(int8_t x, int8_t y, bool startField) {
    Field *new = malloc(sizeof(Field));
    Point nP = {.x=x, .y=y}; // 0, 1

    new->x = x;
    new->y = y;
    new->type = 0;

    // init walls, because of reasons (you don't want
    // to see what happens with out it)
    for(uint8_t i=0; i<4; i++)
        new->neighbors[i] = NULL;

    bool shouldGetDeleted = false;

    if(startField) {
        new->next = NULL;
        new->prev = NULL;
        currentFloor->start = new;
        currentFloor->end = new;
    } else {
        shouldGetDeleted = true;
        for(uint8_t i=0; i<4; i++) {
            Point aP = mapGetAdjacentPositionGlobal(nP, i);
            Field *aF = mapFindField(aP.x, aP.y);
            // does this field has an adjacent field?
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
    } else if (!startField) {
        new->next = NULL;
        new->prev = currentFloor->end;
        currentFloor->end->next = new;
        currentFloor->end = new;
    }

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
                // then create and link field
                Point aP = mapGetAdjacentPositionGlobal(cP, dir);
                mapCreateField(aP.x, aP.y, false);
            }
        }
    }
}

AdjacentScores mapGetAdjacentScores() {
    Field *fieldPtr = currentFloor->start;

    bool flag = false;

    do {
        if(fieldPtr->type != 0)
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
            if(fieldPtr->type != 0)
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
