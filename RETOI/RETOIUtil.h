#ifndef RETOIUtil_h
#define RETOIUtil_h

#include "Arduino.h"

//constant to define the wheel circumference of the bike being used
//used to map rotations to total distance
const int WHEEL_CIRCUMFERENCE = 0;


//the total legs in this current journey
int journeyLength = 0;
//the leg we are at within the journey
int legIndex = 0;
//the current distance traveled within the current leg
int currDistance = 0;

//defines the leg of a journey as a distance + fixed incline
struct Leg { 
   float distance; 
   float incline;
}; 

//the "Journey", which is a series of legs with incline and distance, max of 1000 legs
Leg Journey[1000];

//converts a series of bytes (read in as characters) to a float
float charToFloat(char* value);

void floatToChar(char* output, float value);


#endif