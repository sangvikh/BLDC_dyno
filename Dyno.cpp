#include "Dyno.h"
#include "variables.h"
#include "functions.h"
#include "Logger.h"
#include "VescUart.h"

Logger Logger;
VescUart Brake;   //Brake VESC
VescUart DUT;     //Device under test VESC

Dyno::Dyno(){}
Dyno::~Dyno(){}

void Dyno::begin()
{
  //Define which serial ports to use for VESC's
  Brake.setSerialPort(&Serial1);
  DUT.setSerialPort(&Serial2);
}

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
  else
  {
    rpm = 0.0;
  }
  
  //Set VESC ouputs, set to 0 to disable writing.
  if (rpm > 0)
  {      
    Brake.setRPM(rpm);
  }
  if (current > 0)
  {      
    Brake.setCurrent(current);
  }
  if (currentBrake > 0)
  {      
    Brake.setBrakeCurrent(currentBrake);
  }

  //Get data from vesc's
  Brake.getVescValues();
  //DUT.getVescValues();
  rpmActual = Brake.data.rpm;
}

void Dyno::dynoTest()
{
  float logData[] = {rpmActual, torque};
  Logger.log(logData);
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
