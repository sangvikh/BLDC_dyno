#include "Dyno.h"
#include "variables.h"
#include "functions.h"
#include "Logger.h"

Logger Logger;

Dyno::Dyno(){}
Dyno::~Dyno(){}

void Dyno::startDynoTest()
{
  if (tempTest_ == 0)
  {
    Logger.setFileName("dyno.txt");
    Logger.begin();
    dynoTest_ = 1;
  }
}

void Dyno::startTempTest()
{
  if (dynoTest_ == 0)
  {
    tempTest_ = 1;
  }
}

void Dyno::update()
{
  if (dynoTest_ == 1)
  {
    dynoTest();
  }
  else if (tempTest_ == 1)
  {
    tempTest();
  }
}

void Dyno::dynoTest()
{
  float logData[] = {rpmActual, torque, cycleTime};
  unsigned int length = sizeof(logData)/sizeof(float);
  Logger.log(logData, length);
  rpmSet = 8000.0;
  ramp(rpmSet, 10.0, rpmSet, rpm);
  if (rpm >= rpmSet)
  {
    dynoTest_ = 0;
    rpmSet = 0;
    rpm = 0;
    Logger.end();
  }
}

void Dyno::tempTest()
{
  //Start logging
  //Brake - max current
  //DUT - ramp current
}
