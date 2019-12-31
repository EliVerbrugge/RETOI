/************************************************
 * RETOI - Arduino sketch to receive data points from
 * the desktop utility
 ***********************************************/

#include "RETOI.h"

void setup()
{
  Serial.begin(115200);
  Serial.flush();
}

void loop() 
{
  //monitor serial port to check for new route being loaded in
  if(Serial.available())
  {
    int read = Serial.read();
    Serial.write((char)read);
    //the 'S' character indicates that a new route is about to be loaded onto the arduino
    if((char)read == 'S')
    {
      initRoute();
    }
    else if((char)read == 'R')
    {
      runRoute();
    }
  }
}


void initRoute()
{
  //read in 4 points to start so that there is a buffer
  for(int i = 0; i < 4; i++)
  {
    readInPoint();
  }
}

void runRoute()
{
  while(true)
  {
    Serial.write('N');
    Serial.print(JourneyBuffer[0].distance);
    Serial.print(JourneyBuffer[0].incline);
    readInPoint();
    delay(1000);
  }
}

//read in a stream of data of the form distance, incline+.....+distance, incline
//we assume each piece of data has 2 bytes
void readInPoint()
{
  int read;
  int16_t dist;
  int16_t incline;

  //read in the first 2 bytes, which are for the distance
  while(!Serial.available());
  read = Serial.read();
  dist = read;

  while(!Serial.available());
  read = Serial.read();
  dist = read << 8 | dist;

  //read in the next 2 bytes, which are for the incline
  while(!Serial.available());
  read = Serial.read();
  incline = read;
  
  while(!Serial.available());
  read = Serial.read();
  incline = read << 8 | incline;

  Leg newLeg;
  newLeg.distance = dist;
  newLeg.incline = incline;

  JourneyBuffer.push(newLeg);
}
