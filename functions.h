#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Ramps output to input with ramp time per range
void ramp(float in, float rampTime, float range, float cycleTime, float& out);

//Soft cutoff function for current/RPM
void cutoff(float actualRPM, float maxRPM, float maxCurrent);

//Maps long to float
float mapf(long x, long x0, long x1, float y0, float y1);

#endif
