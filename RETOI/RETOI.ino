/************************************************
 * RETOI - Arduino sketch to receive data points from
 * the desktop utility
 * - Eli Verbrugge, 2019
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
      loadPoints();
    }
    //the 'R' character indicates that the route should begin
    else if((char)read == 'R')
    {
      runRoute();
    }
  }
}

//read in a stream of data of the form distance, incline+.....+distance, incline
//we assume each piece of data has 2 bytes, and the amount of points is also communicated
void loadPoints()
{
  //wait till we have available bytes to read
  while(!Serial.available());

  //read in the first character which will indicate the amount of points in the journey
  int read = Serial.read();
  journeyLength = read;
  while(!Serial.available());
  read = Serial.read();
  journeyLength = read << 8 | journeyLength;

  Serial.print(journeyLength);

  int16_t dist = 0;
  int16_t incline = 0;
  for(int i = 0; i < journeyLength; i++)
  {
    //read in the first 2 bytes, which are for the distance
    while(!Serial.available());
    read = Serial.read();
    dist = read;

    while(!Serial.available());
    read = Serial.read();
    dist = read << 8 | dist;

    Journey[i].distance = dist;
    //Serial.print(dist);

    //read in the next 2 bytes, which are for the incline
    while(!Serial.available());
    read = Serial.read();
    incline = read;
    
    while(!Serial.available());
    read = Serial.read();
    incline = read << 8 | incline;

    Journey[i].incline = incline;
    //Serial.print(incline);
  }
  Serial.print(Journey[0].distance);

}

void runRoute()
{

}
