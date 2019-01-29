#include "functions.h"
#include <Arduino.h>
#include <EEPROM.h>

void ramp(float in, float rampTime, float cycleTime, float range, float& out)
{
  if (in > out) {out = out + cycleTime/rampTime*range;}
  if (in < out) {out = out - cycleTime/rampTime*range;}
}

void cutoff(float actualRPM, float maxRPM, float maxCurrent)
{
  //do something
}

float mapf(long x, long x0, long x1, float y0, float y1)
{
  return y0+((float)x-(float)x0)*(y1-y0)/((float)x1-(float)x0);
}
