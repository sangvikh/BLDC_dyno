#include "LoadCell.h"
#include "HX711-multi.h"
#include "functions.h"
#include "EEPROMAnything.h"

//HX711 scale setup
#define CLK 5      // clock pin to the load cell amp
byte DOUTS[] = {6, 15};    //data pins
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);

LoadCell::LoadCell(){}
LoadCell::~LoadCell(){}

void LoadCell::zero(unsigned char i)
{
  scales.tare(100,0);
  zeroValue_[i] = overflowFix(scales.get_offset(i));
}

void LoadCell::span(unsigned char i)
{
  scales.tare(100,0);
  spanValue_[i] = overflowFix(scales.get_offset(i));
}

void LoadCell::tare()
{
  scales.tare(100,0);
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    tareValue_[i] = zeroValue_[i] - overflowFix(scales.get_offset(i));
  }
}

void LoadCell::refresh()
{
  if (scales.is_ready())
  {
    scales.readRaw(rawValue_);
    for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
    {
      rawValue_[i] = overflowFix(rawValue_[i]);
    }
    scaleValues();
  }
}

float LoadCell::getTorque(unsigned char lc1, unsigned char lc2)
{
  return radius_*(scaledValue_[lc1]-scaledValue_[lc2])*9.81;
}

void LoadCell::setCalibrationMass(float mass)
{
  calibrationMass_ = mass;
}

float LoadCell::getCalibrationMass()
{
  return calibrationMass_;
}

void LoadCell::saveCalibration()
{
  int i = 0;
  i += EEPROM_writeAnything(i, zeroValue_);
  i += EEPROM_writeAnything(i, spanValue_);
  i += EEPROM_writeAnything(i, calibrationMass_);
}

void LoadCell::loadCalibration()
{
  int i = 0;
  i += EEPROM_readAnything(i, zeroValue_);
  i += EEPROM_readAnything(i, spanValue_);
  i += EEPROM_readAnything(i, calibrationMass_);
}

void LoadCell::scaleValues()
{
  for (unsigned int i = 0; i < CHANNEL_COUNT; i++)
  {
    scaledValue_[i] = mapf((rawValue_[i] + tareValue_[i]), zeroValue_[i], spanValue_[i], 0.0, calibrationMass_);
  }
}

long LoadCell::overflowFix(long in)
{
  long out = in;
  if (in > 0x400000)
  {
    out = in - 0x800000;
  }
  return out;
}
