#include "functions.h"
#include "variables.h"
#include <Arduino.h>
#include <EEPROM.h>

void ramp(float in, float rampTime, float range, float& out)
{
  if (in > out) {out = out + cycleTime/rampTime*range;}
  else if (in < out) {out = out - cycleTime/rampTime*range;}
  else {out = in;}      //Fixed not reaching exact setpoint bug
}

void cutoff(float actualRPM, float maxRPM, float maxCurrent)
{
  //do something
}

float mapf(long x, long x0, long x1, float y0, float y1)
{
  return y0+((float)x-(float)x0)*(y1-y0)/((float)x1-(float)x0);
}
