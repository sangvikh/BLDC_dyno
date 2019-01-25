#include "LoadCell.h"
#include "HX711-multi.h"
#include "config.h"


//HX711 scale setup
#define CLK 5      // clock pin to the load cell amp
byte DOUTS[1] = {6};    //data pins
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);

LoadCell::LoadCell(){}
LoadCell::~LoadCell(){}

void LoadCell::zero()
{
  //
}

void LoadCell::span()
{
  //
}

void LoadCell::tare()
{
  //
}

void LoadCell::refresh()
{
  if (scales.is_ready())
  {
    for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
    {
      scales.readRaw(&rawValue_[i]);
    }
  }
}
