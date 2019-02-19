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
  else
  {
    rpm = 0.0;
  }
}

void Dyno::dynoTest()
{
  rpmSet = 10000.0;
  ramp(rpmSet, 15.0, rpmSet, rpm);
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
