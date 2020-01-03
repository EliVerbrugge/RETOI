#ifndef RETOI_h
#define RETOI_h

#include "Arduino.h"
//using https://github.com/rlogiacco/CircularBuffer for Circular Buffer
//will be rewritten in future release
#include <CircularBuffer.h>

//constant to define the wheel circumference of the bike being used
//used to map rotations to total distance, units are meters
const float WHEEL_CIRCUMFERENCE = 1;

//the current distance traveled within the current leg
float currDistance = 0;
//the current reading of the light sensor
uint8_t sensorReading = 0;
//indicates whether the route is running
bool running = false;


//defines the leg of a journey as a distance + fixed incline
struct Leg { 
   int16_t distance; 
   int16_t incline;
}; 

const int SensorPin = A0;
const int IndicatorPin = 3;
bool sensorTriggeredFlag;

//the "Journey", which is a series of legs with incline and distance, buffer of 4 values
CircularBuffer<Leg,4> JourneyBuffer;
#endif
