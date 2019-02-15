#include "Dyno.h"
#include "CycleTime.h"
#include "VescUart.h"
#include "variables.h"
#include "functions.h"

Dyno::Dyno()
{
  
}
Dyno::~Dyno(){}

void Dyno::startDynoTest()
{
  if (tempTest_ == 0)
  {
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
  if (tempTest_ == 1)
  {
    tempTest();
  }
  //Now update outputs (Should be global)
}

void dynoTest()
{
  //Start logging
  //DUT - max current
  //Ramp brake
}

void Dyno::tempTest()
{
  //Start logging
  //Brake - max current
  //DUT - ramp current
}
