#include "Dyno.h"
#include "variables.h"
#include "functions.h"
#include "Logger.h"
#include "arduino.h"

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
  startTime_ = millis();
  float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual};
  unsigned int length = sizeof(logData)/sizeof(float);
  Logger.log(logData, length);
  DUTduty = 0.1;
  currentSet = 50.0;
  ramp(currentSet, 20.0, currentSet, current);
  if (current >= currentSet)
  {
    dynoTest_ = 0;
    rpmSet = 0;
    rpm = 0;
    currentBrakeSet = 0;
    currentBrake = 0;
    DUTduty = 0;
    DUTcurrent = 0;
    Logger.end();
  }
}

void Dyno::tempTest()
{
  startTime_ = millis();
  //Start logging
  //Brake - max current
  //DUT - ramp current
}

int Dyno::poleCheck()
{
  startTime_ = millis();
  rpmSet = 1000.0;
  int poleCount = 0;
  if (millis() - startTime_ >= 500)
  {
    int poleCount = (int)round(rpmActual/rpm);
  }
  return poleCount;
}
