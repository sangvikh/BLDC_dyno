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

  //Returns cycle time in seconds
  float getCycleTime()
  {
    return cycleTimeS_;
  }

  //Returns true if cycle time has passed
  bool checkTime()
  {
    if (micros()-lastTime_ >= cycleTime_)
    {
      lastTime_ = micros();
      return 1;
    }
    else
    {
      return 0;
    }
  }

  //Ramp time per range
  void ramp(float in, float rampTime, float range, float& out)
  {
    if (in > out) {out = out + cycleTimeS_/rampTime*range;}
    if (in < out) {out = out - cycleTimeS_/rampTime*range;}
  }
  
private:
  unsigned long lastTime_;  //Last timestamp
  unsigned long cycleTime_; //Cycle time in microseconds
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
