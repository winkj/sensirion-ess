#include <sensirion_ess.h>

SensirionESS ess;

int repetitions  =    3;
int fadeDelay    =    5;
int stepDelay    =  100;
int sectionDelay = 1000;

void setup()
{
  // this is needed to set the pinModes
  ess.initSensors();
}

void loop()
{
  // -- Sensirion::setLedRYG()
  for (int i = 0; i < repetitions; ++i) {
    ess.setLedRYG(1, 0, 0); delay(stepDelay);
    ess.setLedRYG(0, 1, 0); delay(stepDelay);
    ess.setLedRYG(0, 0, 1); delay(stepDelay);
  }
  ess.setLedRYG(0, 0, 0);
  delay(sectionDelay);

  // -- manual access
  for (int i = 0; i < repetitions; ++i) {
    digitalWrite(SensirionESS::LED_RED, HIGH); delay(stepDelay);
    digitalWrite(SensirionESS::LED_YEL, HIGH); delay(stepDelay);
    digitalWrite(SensirionESS::LED_GRN, HIGH); delay(stepDelay);
    digitalWrite(SensirionESS::LED_RED,  LOW); delay(stepDelay);
    digitalWrite(SensirionESS::LED_YEL,  LOW); delay(stepDelay);
    digitalWrite(SensirionESS::LED_GRN,  LOW); delay(stepDelay);
  }
  ess.setLedRYG(0, 0, 0);
  delay(sectionDelay);

  // -- analogWrite() - requires pins 9-11 to be PWM pins
  for (int i = 0; i < repetitions; ++i) {
    for (int j = 0; j < 255; ++j) {
      analogWrite(SensirionESS::LED_RED, j);
      analogWrite(SensirionESS::LED_YEL, j);
      analogWrite(SensirionESS::LED_GRN, j);
      delay(fadeDelay);
    }
    for (int j = 255; j >= 0; --j) {
      analogWrite(SensirionESS::LED_RED, j);
      analogWrite(SensirionESS::LED_YEL, j);
      analogWrite(SensirionESS::LED_GRN, j);
      delay(fadeDelay);
    }
  }
  ess.setLedRYG(0, 0, 0);
  delay(sectionDelay);
}
