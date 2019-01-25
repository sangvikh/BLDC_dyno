#include "LoadCell.h"
#include "HX711-multi.h"

//HX711 scale setup
#define CLK 5      // clock pin to the load cell amp
byte DOUTS[1] = {6};    //data pins
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);

float mapf(long x, long x0, long x1, float y0, float y1)
{return y0+((float)x-x0)*(y1-y0)/(x1-x0);}

LoadCell::LoadCell(){}
LoadCell::~LoadCell(){}

void LoadCell::zero(unsigned char i)
{
  scales.tare(100,0);
  zeroValue_[i] = scales.get_offset(i);
}

void LoadCell::span(unsigned char i)
{
  scales.tare(100,0);
  spanValue_[i] = scales.get_offset(i);
}

void LoadCell::tare()
{
  scales.tare(100,0);
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    tareValue_[i] = zeroValue_[i] - scales.get_offset(i);
  }
}

void LoadCell::refresh()
{
  if (scales.is_ready())
  {
    scales.readRaw(rawValue_);
  }
    scaleValues();
}

float LoadCell::getTorque(unsigned char lc1, unsigned char lc2)
{
  return radius_*(scaledValue_[lc1]+scaledValue_[lc2]);
}

void LoadCell::scaleValues()
{
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    scaledValue_[i] = mapf((rawValue_[i] + tareValue_[i]), zeroValue_[i], spanValue_[i], 0.0, calibrationMass_);
  }
}
