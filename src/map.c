#include "map.h"


void mapInit() {
    memoryCounter = 0;

    heading = NORTH;

    currentFloor = malloc(sizeof(struct Floor));

    currentFloor->start = mapCreateField(0, 0, true);
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

    if(!startField) {
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

FieldLinkedListElement *mapFloorTo1DList() {

    // the open list contains all fields of which the neighbor fields didn't got tracked yet
    memoryCounter++;
    FieldLinkedListElement *openStart = malloc(sizeof(FieldLinkedListElement));
    openStart->field = currentFloor->start;
    openStart->next = NULL;
    openStart->prev = NULL;
    
    FieldLinkedListElement *openEnd = openStart;
    openEnd->field = currentFloor->start;
    openEnd->next = NULL;
    openEnd->prev = NULL;

    // the closed list contains all fields where already all neighbor fields got tracked
    FieldLinkedListElement* closedStart = NULL;
    FieldLinkedListElement* closedEnd = NULL;

    // as long as there are elements in open there must be fields in the map that aren't in the 1D list
    while(openStart != NULL) {

        // remove last field from open list, ...
        struct FieldLinkedListElement *removedOpen;
        removedOpen = openEnd;

        

        if(removedOpen->prev == NULL) {
            openStart = NULL;
            openEnd = NULL;
        } else {
            openEnd = removedOpen->prev;
            openEnd->next = NULL;
        }

        // ... and add the neighbor fields to the open list ...
        for(uint8_t i=0; i<4; i++) {

            // ... as long as this neighbor exists, ...
            if(removedOpen->field->neighbors[i] != NULL) {

                // ... and is not already contained inside the open list ...
                bool notOpenAlready = true;
                if(openStart != NULL) {
                    FieldLinkedListElement *openPtr;
                    openPtr = openStart;
                    do {
                        if(openPtr->field == removedOpen->field->neighbors[i])
                            notOpenAlready = false;
                        openPtr = openPtr->next;
                    } while((openPtr != NULL) & notOpenAlready);
                }

                // ... or the closed list
                bool notClosedAlready = true;
                if(closedStart != NULL) {
                    FieldLinkedListElement *closedPtr;
                    closedPtr = closedStart;
                    do {
                        if(closedPtr->field == removedOpen->field->neighbors[i])
                            notClosedAlready = false;
                        closedPtr = closedPtr->next;
                    } while((closedPtr != NULL) & notClosedAlready);
                }

                if(notClosedAlready && notOpenAlready)
                {
                    memoryCounter++;
                    FieldLinkedListElement *new = malloc(sizeof(FieldLinkedListElement));
                    if(new == NULL) {
                        
                    }
                    new->field = removedOpen->field->neighbors[i];
                    new->prev = openEnd;
                    openEnd->next = new;
                    openEnd = new;
                    if(openStart == NULL)
                        openStart = new;
                    new->next = NULL;
                }
            }
        }

        // in the end: add the removed element to the closed list
        if(closedStart == NULL) {
            closedStart = removedOpen;
            closedStart->prev = NULL;
            closedStart->next = NULL;
            closedEnd = closedStart;
        } else {
            closedEnd->next = removedOpen;
            removedOpen->prev = closedEnd;
            closedEnd = removedOpen;
        }
    }

    return closedStart;
}

Field* mapFindField(int8_t x, int8_t y) {
    FieldLinkedListElement *fieldStart = mapFloorTo1DList();
    FieldLinkedListElement *fieldPtr = fieldStart;
    Field *rtn = NULL;

    do {
        if((fieldPtr->field->x==x) && (fieldPtr->field->y==y))
            rtn = fieldPtr->field;
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    mapFreeLinkedList(fieldStart);

    return rtn;
}

void mapFreeLinkedList(FieldLinkedListElement *fieldPtr) {
    FieldLinkedListElement *next;

    do {
        next = fieldPtr->next;
        serialPrintInt(50000);
        memoryCounter--;
        free(fieldPtr);
        fieldPtr = next;
    } while(fieldPtr != NULL);
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
    FieldLinkedListElement *fieldStart = mapFloorTo1DList();
    FieldLinkedListElement *fieldPtr = fieldStart;

    bool flag = false;

    do {
        if(fieldPtr->field->type != 0)
            fieldPtr->field->score = 0;
        else {
            fieldPtr->field->score = 127;
            flag = true;
        }
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    if(flag==false && currentField!=currentFloor->start) {
        fieldPtr = fieldStart;
        currentFloor->start->type = 0;
        do {
            if(fieldPtr->field->type != 0)
                fieldPtr->field->score = 0;
            else
                fieldPtr->field->score = 127;
            fieldPtr = fieldPtr->next;
        } while(fieldPtr != NULL);
    }

    if((currentField!=currentFloor->start) || (currentField==currentFloor->start && flag==true)) {

        int8_t i=127;

        while(currentField->score==0) {
            fieldPtr = fieldStart;
            do {
                if(fieldPtr->field->score == 0) {
                    for(uint8_t dir=0; dir<4; dir++) {
                        if(fieldPtr->field->neighbors[dir]!=NULL) {
                            if(fieldPtr->field->neighbors[dir]->score == i) {
                                fieldPtr->field->score = i-1;
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

    mapFreeLinkedList(fieldStart);

    mapSender();

    return aScores;
}

void mapSender() {
    FieldLinkedListElement *fieldStart = mapFloorTo1DList();
    FieldLinkedListElement *fieldPtr = fieldStart;

    serialPrintNL();
    serialPrintNL();
    serialPrintInt(currentField->x);
    serialPrintInt(currentField->y);
    serialPrintNL();

    do {
        serialPrintInt(fieldPtr->field->x);
        serialPrintInt(fieldPtr->field->y);
        serialPrintInt(fieldPtr->field->type);
        serialPrintInt(fieldPtr->field->score);
        for(uint8_t dir=0; dir<4; dir++)
            serialPrintInt(((fieldPtr->field->neighbors[dir]==NULL)?1:0));
        serialPrintNL();
        fieldPtr = fieldPtr->next;
    } while(fieldPtr != NULL);

    mapFreeLinkedList(fieldStart);

    serialPrintNL();
}
