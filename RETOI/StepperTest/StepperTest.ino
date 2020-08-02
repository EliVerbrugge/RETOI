#include <Stepper.h>

#define DEGREES_PER_STEP 1.8
#define STEPS_PER_REV 200

//update these pins later
Stepper stepperMotor(STEPS_PER_REV, 8, 9, 10, 11);

void setup()
{
    Serial.begin(9600);
    Serial.println("Starting program");
    stepperMotor.setSpeed(60);
}

void loop()
{
    stepperMotor.step(-2/DEGREES_PER_STEP);
    delay(500);
}
