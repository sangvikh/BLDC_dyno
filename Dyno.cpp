#include "Dyno.h"
#include "config.h"
#include "variables.h"
#include "functions.h"
#include "Logger.h"
#include "arduino.h"
#include "VescUart.h"
#include "PID.h"

Logger Logger;
VescUart Brake;   //Brake VESC
VescUart DUT;     //Device under test VESC
PID PID;

Dyno::Dyno(){}
Dyno::~Dyno(){}

void Dyno::begin()
{
  //Setup serial to VESC's
  Serial1.begin(serialBaud);
  Serial2.begin(serialBaud);

  //Define which serial ports to use for VESC's
  Brake.setSerialPort(&Serial1);
  DUT.setSerialPort(&Serial2);
}

void Dyno::startDynoTest()
{
  if (testState_ == 0)
  {
    startTime_ = millis();
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
    state_ = 0;
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
    startTime_ = millis();
    testState_ = 2;
    Logger.setFileName("temp.txt");
    Logger.begin();
    DUTcurrentSet = 100.0;
    currentBrake = 100.0;
    PID.setPID(5.0, 0.05, 0);
    PID.reset();
  }
}

void Dyno::startPoleCheck()
{
  if (testState_ == 0)
  {
    startTime_ = millis();
    testState_ = 3;
  }
}

int Dyno::getPolePairs()
{
  return polePairs_;
}

void Dyno::update()
{
  //Get data from VESC's
  Brake.getVescValues();
  DUT.getVescValues();
  rpmActual = Brake.data.rpm/7;
  motorCurrent = Brake.data.avgMotorCurrent;
  DUTmotorCurrent = DUT.data.avgMotorCurrent;
  dutyActual = Brake.data.dutyCycleNow;
  DUTdutyActual = DUT.data.dutyCycleNow;
  inputCurrent = Brake.data.avgInputCurrent;
  DUTinputCurrent = DUT.data.avgInputCurrent;
  inputVoltage = Brake.data.inpVoltage;
  DUTinputVoltage = DUT.data.inpVoltage;
    
  //Update the current running program
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
    case 3:
      poleCheck();
      break;
    case 4:
      PID.pid(1000.0, rpmActual, DUTcurrent);
      DUT.setCurrent(DUTcurrent);
      Serial.println(rpmActual);
      Serial.println(PID.getIntegral());
      Serial.println(PID.getdt(),4);
      Serial.println("--------------------");
  }

  //Update outputs
  //Set VESC ouputs, set to 0 to disable writing.
  if (rpm > 0)
  {      
    Brake.setRPM(rpm*7);
  }
  if (current > 0)
  {      
    Brake.setCurrent(current);
  }
  if (currentBrake > 0)
  {      
    Brake.setBrakeCurrent(currentBrake);
  }
  if (DUTrpm > 0)
  {      
    DUT.setRPM(DUTrpm*7);
  }
  if (DUTduty > 0)
  {      
    DUT.setDuty(DUTduty);
  }
}

void Dyno::dynoTest()
{
  startTime_ = millis();
  float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual, DUTtemp};
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
  switch(state_)
  {
    case 0:
    {
      ramp(DUTcurrentSet, 600, maxCurrent_, DUTcurrent);
      DUT.setCurrent(DUTcurrent + (maxTemp_-DUTtemp)*100.0/maxCurrent_);
      if (DUTtemp > maxTemp_)
      {
        state_ = 1;
        DUTnominalCurrent_ = DUTmotorCurrent;
        startTime_ = millis();
      }
      break;
    }
    case 1:
    {
      float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual, DUTtemp};
      unsigned int length = sizeof(logData)/sizeof(float);
      Logger.log(logData, length);
      PID.pid(maxTemp_, DUTtemp, DUTcurrent);
      DUT.setCurrent(DUTnominalCurrent_ + DUTcurrent);
      if (millis() - startTime_ >= 300000)
      {
        DUTnominalCurrent_ = DUTmotorCurrent;
        stopTest();
      }
      break;
    }
  }
}

void Dyno::poleCheck()
{
  DUTrpmSet = 10000;
  Brake.setRPM(DUTrpmSet);
  if (millis() - startTime_ >= 1000)
  {
    polePairs_ = (int)round(DUTrpmSet/rpmActual);
    stopTest();
  }
}

void Dyno::pidRPM()
{
  PID.setPID(0.01, 0.01, 0);
  testState_ = 4;
}
