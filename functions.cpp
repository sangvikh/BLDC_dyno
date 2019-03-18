#include "functions.h"
#include "variables.h"
#include <Arduino.h>
#include <EEPROM.h>

void ramp(float in, float rampTime, float range, float& out)
{
  if (out < in) {out += cycleTime/rampTime*range;}
  else if (out > in)
  {
    out -= cycleTime/rampTime*range;
    if (out < in) {out = in;}         //This line checks if the setpoint has been overshot
  }
}

void cutoff(float actualRPM, float maxRPM, float maxCurrent)
{
  //do something
}

float mapf(long x, long x0, long x1, float y0, float y1)
{
  return y0+((float)x-(float)x0)*(y1-y0)/((float)x1-(float)x0);
}
