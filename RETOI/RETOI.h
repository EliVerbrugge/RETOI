#ifndef RETOI_h
#define RETOI_h

#include "Arduino.h"
//using https://github.com/rlogiacco/CircularBuffer for Circular Buffer
//will be rewritten in future release
#include <CircularBuffer.h>

//constant to define the wheel circumference of the bike being used
//used to map rotations to total distance
const int16_t WHEEL_CIRCUMFERENCE = 0;


//the total legs in this current journey
int16_t journeyLength = 0;
//the leg we are at within the journey
int16_t legIndex = 0;
//the current distance traveled within the current leg
int16_t currDistance = 0;

//defines the leg of a journey as a distance + fixed incline
struct Leg { 
   int16_t distance; 
   int16_t incline;
}; 

//the "Journey", which is a series of legs with incline and distance, buffer of 4 values
CircularBuffer<Leg,4> JourneyBuffer;
#endif
