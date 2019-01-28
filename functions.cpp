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

void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  //Write the 4 bytes into the eeprom memory.  EEPROM.write(address, four);
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

float mapf(long x, long x0, long x1, float y0, float y1)
{
  return y0+((float)x-(float)x0)*(y1-y0)/((float)x1-(float)x0);
}
