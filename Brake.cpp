#include "Brake.h"
#include <Arduino.h>

class CycleTime
{
public:
  CycleTime()
  {}
  ~CycleTime()
  {}

  //Sets the cycle time in milliseconds
  void setCycleTime(unsigned long cycletime)
  {
    cycleTime_ = cycletime;
  }

  float getCycleTime()
  {
    return (float)cycleTime_/1000;
  }

  //Returns true if cycle time has passed
  bool checkTime()
  {
    if (millis() >= lastTime_)
    {
      lastTime_ = millis();
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
  unsigned long cycleTime_ = 10; //Cycle time in microseconds
  float cycleTimeS_ = 0.01; //Cycle time in seconds
};

//Help functions
