#include "Brake.h"
#include <Arduino.h>

class CycleTime
{
public:
  CycleTime(){};
  ~CycleTime(){};

  //Sets the cycle time in seconds
  void setCycleTime(float cycletime)
  {
    cycleTime_ = (unsigned long)cycletime*1000000;
    cycleTimeS_ = cycletime;
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
  unsigned long lastTime_;
  unsigned long cycleTime_ = 10000; //Cycle time in microseconds
  float cycleTimeS_ = 0.01; //Cycle time in seconds
};

//Help functions
