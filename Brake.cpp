#include "Brake.h"
#include <Arduino.h>

class CycleTime
{
public:
  //CycleTime();
  //~CycleTime();

  //Sets the cycle time in seconds
  void setCycleTime(float cycletime)
  {
    cycleTime_ = (long)cycletime*1000000;
  }

  float getCycleTime()
  {
    return (float)cycleTime_/1000000;
  }

  //Returns true if cycle time has passed
  bool checkTime()
  {
    if (micros() > lastTime_)
    {
      lastTime_ = micros();
      return 1;
    }
    else
    {
      return 0;
    }
  }

  //Ramp time in seconds per unit
  void ramp(float in, float rampTime, float& out)
  {
    if (in > out) {out = out + cycleTimeS_/rampTime;}
    if (in < out) {out = out - cycleTimeS_/rampTime;}
  }
  
private:
  unsigned long lastTime_;
  unsigned long cycleTime_ = 10000; //Cycle time in microseconds
  float cycleTimeS_ = 0.01; //Cycle time in seconds
};

//Help functions
