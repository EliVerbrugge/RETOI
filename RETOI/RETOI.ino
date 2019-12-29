/************************************************
 * RETOI - Arduino sketch to receive data points from
 * the desktop utility
 * - Eli Verbrugge, 2019
 ***********************************************/

#include "RETOIUtil.h"

void setup()
{
  Serial.begin(115200);
}

void loop() 
{
  //monitor serial port to check for new route being loaded in
  if(Serial.available())
  {
    //the 'S' character indicates that a new route is about to be loaded onto the arduino
    if((char)Serial.read() == 'S')
    {
      loadPoints();
    }
  }
}

//read in a stream of data of the form distance, incline+.....+distance, incline
//we assume each piece of data has 4 bytes, and the amount of points is also communicated
void loadPoints()
{
  //wait till we have available bytes to read
  while(!Serial.available());

  //read in the first character which will indicate the amount of points in the journey
  journeyLength = Serial.read();
  char dist[4];
  char incline[4];

  for(int i = 0; i < journeyLength; i++)
  {
    //read in the first 4 bytes, which are for the distance
    for(int j = 0; j < 4; j++)
    {
      dist[j] = (char)Serial.read();
    }
    Journey[i].distance = charToFloat(dist);

    //read in the next 4 bytes, which are for the incline
    for(int k = 0; k < 4; k++)
    {
      incline[k] = (char)Serial.read();
    }
    Journey[i].incline = charToFloat(incline);
  }
}

void floatToChar(char* output, float value)
{
	union u_tag 
  {
	  byte b[4];
	  float fval;
	} u;

	u.fval = value;

	output[0] = u.b[0];
	output[1] = u.b[1];
	output[2] = u.b[2];
	output[3] = u.b[3];
}

float charToFloat(char* value)
{
	union u_tag 
  {
    byte b[4];
    float fval;
	} u;

  u.b[0] = value[0];
	u.b[1] = value[1];
	u.b[2] = value[2];
	u.b[3] = value[3];

	return u.fval;
}


