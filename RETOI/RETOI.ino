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
  int read;
  while(!Serial.available());
  read = Serial.read();
  total_legs = read;
  
  while(!Serial.available());
  read = Serial.read();
  total_legs = read << 8 | total_legs;

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
    
    /********************************************************
    make motor move according to incline
    JourneyBuffer[0].incline
    ********************************************************/
    stepperMotor.step(JourneyBuffer[0].incline/DEGREES_PER_STEP);

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
      //if we have read in the total number of points, we end the route
      if(currentLeg == total_legs)
      {
        running = false;
        break;
      }
      else
      {
        Serial.write('N');
        currDistance = 0;
      }
    }
    //always read in from serial buffer if there is info available
    if(Serial.available())
    {
      readInPoint();
    }
  }
  //cleanup the run data
  currDistance = 0;
  JourneyBuffer.clear();
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

  //special case, if the distance is negative, we terminate 
  //as that indicates the route is done
  if(dist < 0)
  {
    running = false;
    return;
  }

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
  currentLeg++;
}
