//Ramps output to input with ramp time per range
void ramp(float in, float rampTime, float range, float& out, float cycleTime)
{
  if (in > out) {out = out + cycleTime/rampTime*range;}
  if (in < out) {out = out - cycleTime/rampTime*range;}
}

//Soft cutoff function for current/RPM
void cutoff(float actualRPM, float maxRPM, float maxCurrent)
{
  //do something
}
