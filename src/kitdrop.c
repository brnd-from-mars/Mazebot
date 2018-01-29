//#include "kitdrop.h"
//#include "Servo.cpp"
//#include <Arduino.h>
#include <Servo.h>

   
void kitdropleft()
{ 
 int i;         
 Servo.attach(11);  
 for (i = 85; i > 9; i--) 
 { 
   Servo.write(i);              
   delay(20);                          
 }

 for (i = 9; i < 85; i++) 
 { 
   Servo.write(i);             
   delay(20);                       
 }
 Servo.detach();
 return;
}

void kitdropright()
{
 int i;
 Servo.attach(11);  
 for (i = 85; i < 170; i++) 
 { 
   Servo.write(i);              
   delay(20);                          
  }

  for (i = 170; i > 85; i--) 
  { 
    Servo.write(i);             
    delay(20);                       
  }
 Servo.detach();
 return;
}