#include "map.h"


void mapInit() {
    pos.x = 0;
    pos.y = 0;
    heading = NORTH;

    currentFloor = &arena[0];
    currentField = &((*currentFloor).fields[pos.y][pos.x]);

    (*currentField).visited = 1;
    (*currentField).type = 2;
    (*currentField).wallData = getWallData(heading) & 0x0F;

    for(uint8_t i=0; i<FLOOR_COUNT; i++) {
        backup[i] = arena[i];
    }
}

void mapCreatorForward() {
    /*
     *          pos.X       pos.Y
     * EAST    +1            0
     * SOUTH    0           -1
     * WEST    -1            0
     * NORTH    0           +1
     */

    uint8_t dx = abs(heading-2)-1;
    uint8_t dy = abs(heading-1)-1;

    pos.x += dx;
    pos.y += dy;

    if(pos.x < 0) {
        mapCreatorPushX();
        pos.x++;
    }

    if(pos.y < 0) {
        mapCreatorPushY();
        pos.y++;
    }

    currentField = &((*currentFloor).fields[pos.y][pos.x]);

    (*currentField).visited = 1;
    (*currentField).type = 0;
    (*currentField).wallData = getWallData(heading) & 0x0F;
}

bool mapCreatorRotate(int8_t amount) {
    int8_t tmp = (heading+amount);

    if(tmp<0)
        heading = tmp+4;
    else if(tmp>3)
        heading = tmp-4;
    else
        heading = tmp;

    uint8_t old = (*currentField).wallData;
    uint8_t new = getWallData(heading) & 0x0F;

    return (new==old);
}

void mapCreatorPushX() {
    for(uint8_t y=0; y<FLOOR_SIZE; y++) {
        for(uint8_t x=FLOOR_SIZE-1; x>0; x--) {
            (*currentFloor).fields[y][x] = (*currentFloor).fields[y][x-1];
        }
        (*currentFloor).fields[y][0].visited = 0;
        (*currentFloor).fields[y][0].type = 0;
        (*currentFloor).fields[y][0].wallData = 0;
    }
}

void mapCreatorPushY() {
    for(uint8_t x=0; x<FLOOR_SIZE; x++) {
        for(uint8_t y=FLOOR_SIZE-1; y>0; y--) {
            (*currentFloor).fields[y][x] = (*currentFloor).fields[y-1][x];
        }
        (*currentFloor).fields[0][x].visited = 0;
        (*currentFloor).fields[0][x].type = 0;
        (*currentFloor).fields[0][x].wallData = 0;
    }
}

void mapCreatorBlackField() {
    struct Point frontPoint = mapGetAdjacentPosition(FRONT, true);
    struct Field *frontField = &((*currentFloor).fields[frontPoint.y][frontPoint.x]);

    (*frontField).type = 1;
    (*frontField).visited = 1;
}

struct Point mapGetAdjacentPosition(uint8_t dir, bool allowPush) {
    uint8_t global = (dir + heading + 1)%4;

    uint8_t dx = abs(global-2)-1;
    uint8_t dy = abs(global-1)-1;

    struct Point aPos;

    aPos.x = pos.x + dx;
    aPos.y = pos.y + dy;

    if(aPos.x < 0) {
        if(allowPush) {
            mapCreatorPushX();
            aPos.x = 0;
        } else
            aPos.x = -1;
    }
    if(aPos.x >= FLOOR_SIZE)
        aPos.x = FLOOR_SIZE-1;
    
    if(aPos.y < 0) {
        if(allowPush) {
            mapCreatorPushY();
            aPos.y = 0;
        } else
            aPos.y = -1;
    }
    if(aPos.y >= FLOOR_SIZE)
        aPos.y = FLOOR_SIZE-1;

    return aPos;
}

bool mapAllowRightTurn() {
    struct Point rightPoint = mapGetAdjacentPosition(RIGHT, false);
    
    if((rightPoint.x == -1) || (rightPoint.y == -1))
        return true;

    struct Field *rightField = &((*currentFloor).fields[rightPoint.y][rightPoint.x]);

    if((*rightField).visited && (*rightField).type==1)
        return false;
    else
        return true;
}
