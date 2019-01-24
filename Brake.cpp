#include <Arduino.h>
#include "Brake.h"
#include "variables.h"

class CycleTime
{
public:
  CycleTime(float cycleTime = 0.01)
  {
    cycleTime_ = (unsigned long)(cycleTime*1000000.0);
    cycleTimeS_ = cycleTime;
  };
  ~CycleTime(){};

  //Sets the cycle time in milliseconds
  void setCycleTime(unsigned long cycleTime)
  {
    cycleTime_ = cycleTime*1000;
    cycleTimeS_ = (float)cycleTime/1000;
  }

  //Returns the actual cycle time in seconds
  float getCycleTime()
  {
    return (float)cycleTimeActual_/1000000.0;
  }

  //Returns true if cycle time has passed
  //Only run once per loop
  bool checkTime()
  {
    if (micros()-lastTime_ >= cycleTime_)
    {
      cycleTimeActual_ = micros()-lastTime_;
      lastTime_ = micros();
      return 1;
    }
    else
    {
      return 0;
    }
  }

  //Ramps output to input with ramp time per range
  void ramp(float in, float rampTime, float range, float& out)
  {
    if (in > out) {out = out + getCycleTime()/rampTime*range;}
    if (in < out) {out = out - getCycleTime()/rampTime*range;}
  }
  
private:
  unsigned long lastTime_;  //Last timestamp
  unsigned long cycleTime_; //Cycle time in microseconds
  unsigned long cycleTimeActual_; //Actual cycle time
  float cycleTimeS_; //Cycle time in seconds
};

class SoftCutoff
{
public:
  SoftCutoff(){};
  ~SoftCutoff(){};

  void cutoff(float actualRPM, float maxRPM, float maxCurrent)
  {
    //Something
  }
private:
  float rpm;
  float current;
};

/*
class LoadcellCalibration
{
public:
  //Takes in number of load cells
  LoadcellCalibration(unsigned char count = 1)
  {
    numberOfLoadCells_ = count;
  }
  ~LoadcellCalibration() {}

  void tare()
  {
    for (int i = 0; i < numberOfLoadCells_; i++)
    {
      scales.tare(100, 1000);
      scales.readRaw(&tareValue_);
    }
  }

  void zero()
  {
    //
  }

  void span()
  {
    //
  }
  
private:
  unsigned char numberOfLoadCells_;
  long tareValue_[4];
};
*/
