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
    rpmSet = 10000.0;
    DUTcurrent = 20.0;
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
  ramp(rpmSet, 1, 100, rpm);
  if (rpm == rpmSet || DUTinputCurrent < 0.0)
  {
    //Enda the dyno test, sets all values to 0
    dynoTest_ = 0;
    rpmSet = 0;
    rpm = 0;
    current = 0;
    currentBrakeSet = 0;
    currentBrake = 0;
    DUTduty = 0;
    DUTcurrent = 0;
    DUTrpm = 0;
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
