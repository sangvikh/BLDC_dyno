#include "CycleTime.h"
#include <Arduino.h>
#include "HX711-multi.h"

CycleTime::CycleTime(unsigned long cycleTime)
{
  cycleTime_ = cycleTime*1000;
}
CycleTime::~CycleTime(){};

void CycleTime::setCycleTime(unsigned long cycleTime)
{
  cycleTime_ = cycleTime*1000;
}

float CycleTime::getCycleTime()
{
  return (float)cycleTimeActual_/1000000.0;
}

bool CycleTime::checkTime()
{
  if (micros() - lastTime_ >= cycleTime_)
  {
    cycleTimeActual_ = micros() - lastTime_;
    lastTime_ = micros();
    return 1;
  }
  else
  {
    return 0;
  }
}
