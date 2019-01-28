#include "LoadCell.h"
#include "HX711-multi.h"
#include "functions.h"

//HX711 scale setup
#define CLK 5      // clock pin to the load cell amp
byte DOUTS[1] = {6};    //data pins
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);

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

void LoadCell::saveCalibration()
{
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    EEPROMWritelong(4*i,zeroValue_[i]);
    EEPROMWritelong(16+4*i,spanValue_[i]);
  }
}

void LoadCell::loadCalibration()
{
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    zeroValue_[i] = EEPROMReadlong(4*i);
    spanValue_[i] = EEPROMReadlong(16+4*i);
  }
}

void LoadCell::scaleValues()
{
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    scaledValue_[i] = mapf((rawValue_[i] + tareValue_[i]), zeroValue_[i], spanValue_[i], 0.0, calibrationMass_);
  }
}
