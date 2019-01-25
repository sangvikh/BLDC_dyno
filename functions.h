//Function to map various datatypes to float
//Overloaded datatypes

float floatMap(long x, float x0, float x1, float y0, float y1)
{return y0+((float)x-x0)*(y1-y0)/(x1-x0);}

float floatMap(int x, float x0, float x1, float y0, float y1)
{return y0+((float)x-x0)*(y1-y0)/(x1-x0);}

float floatMap(char x, float x0, float x1, float y0, float y1)
{return y0+((float)x-x0)*(y1-y0)/(x1-x0);}

float floatMap(float x, float x0, float x1, float y0, float y1)
{return y0+((float)x-x0)*(y1-y0)/(x1-x0);}


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
