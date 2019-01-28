#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Ramps output to input with ramp time per range
void ramp(float in, float rampTime, float range, float cycleTime, float& out);

//Soft cutoff function for current/RPM
void cutoff(float actualRPM, float maxRPM, float maxCurrent);

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value);

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address);

//Maps long to float
float mapf(long x, long x0, long x1, float y0, float y1);

#endif
