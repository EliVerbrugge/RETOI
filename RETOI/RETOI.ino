/************************************************
 * RETOI - Arduino sketch to receive data points from
 * the desktop utility
 ***********************************************/

#include "RETOI.h"

void setup()
{
  Serial.begin(115200);
  Serial.flush();
  pinMode(IndicatorPin, OUTPUT);
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
      running = true;
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
  while(running)
  {
    sensorReading = analogRead(SensorPin);
    if(sensorReading > 3)
    {
      analogWrite(IndicatorPin,250);
      sensorTriggeredFlag = false;
    }
    else if(sensorReading <= 3 && sensorTriggeredFlag == false)
    {
      analogWrite(IndicatorPin, 0);
      sensorTriggeredFlag = true;
      currDistance += WHEEL_CIRCUMFERENCE;
      Serial.println(JourneyBuffer[0].distance);
      Serial.println(currDistance);
      delay(500);
    }

    if(currDistance > JourneyBuffer[0].distance)
    {
      Serial.write('N');
      //Serial.print(JourneyBuffer[0].distance);
      //Serial.print(JourneyBuffer[0].incline);
      readInPoint();
      currDistance = 0;
    }
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
  //the distance cannot be negative, so terminate if we receive a negative point
  if(read < -1)
  {
    running = false;
    return;
  }
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
