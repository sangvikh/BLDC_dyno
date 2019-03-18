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
  if (testState_ == 0)
  {
    testState_ = 1;
    Logger.setFileName("dyno.txt");
    Logger.begin();
    rpmSet = 10000.0;
    DUTduty = 0.25;
  }
}

void Dyno::stopTest()
{
    //Enda the dyno test, sets all values to 0
    testState_ = 0;
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

void Dyno::startTempTest()
{
  if (testState_ == 0)
  {
    testState_ = 2;
  }
}

void Dyno::update()
{
  switch(testState_)
  {
    case 0:
      //Do nothing
      break;
    case 1:
      dynoTest();
      break;
    case 2:
      tempTest();
      break;
  }
}

void Dyno::dynoTest()
{
  startTime_ = millis();
  float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual};
  unsigned int length = sizeof(logData)/sizeof(float);
  Logger.log(logData, length);
  ramp(rpmSet, 1, 100, rpm);

  //Stop criteria
  if (rpm == rpmSet || DUTinputCurrent < 0.0 || DUTdutyActual >= 0.95 || dutyActual >= 0.95)
  {
    stopTest();
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
