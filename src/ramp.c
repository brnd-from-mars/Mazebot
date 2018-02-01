#include "ramp.h"

//Rückgabe 1 = Rampe hoch, 0 = Ebene, -1 = Rampe runter

 int rampstatus(){
    int gyroval=analogRead(3);
    int rampupmaybe;
    int rampdownmaybe;
   if(gyroval > RAMPUPVAL){
        rampupmaybe++;
        rampdownmaybe=0;
    }
    else if (gyroval < RAMPDOWNVAL){
        rampdownmaybe++;
        rampupmaybe=0;
    }
    
    if(rampupmaybe > 10){
        return 1;
    }
    else if (rampdownmaybe > 10){
        return -1;
    } 
    else { 
        return 0;
    }
}