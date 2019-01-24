//Function to map various datatypes to float
//
float floatMap(long x, float x0, float x1, float y0, float y1)
{
  return y0+((float)x-x0)*(y1-y0)/(x1-x0);
}

float floatMap(int x, float x0, float x1, float y0, float y1)
{
  return y0+((float)x-x0)*(y1-y0)/(x1-x0);
}

float floatMap(char x, float x0, float x1, float y0, float y1)
{
  return y0+((float)x-x0)*(y1-y0)/(x1-x0);
}

float floatMap(float x, float x0, float x1, float y0, float y1)
{
  return y0+((float)x-x0)*(y1-y0)/(x1-x0);
}

//Next function
//
