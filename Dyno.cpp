#include "Dyno.h"
#include "config.h"
#include "variables.h"
#include "functions.h"
#include "arduino.h"
#include "VescUart.h"
#include "PID.h"

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

void Dyno::stopTest()
{
    //Ends the dyno test, sets all values to 0
    testState_ = IDLE;
    rpmSet = 0;
    rpm = 0;
    current = 0;
    currentBrakeSet = 0;
    currentBrake = 0;
    DUTduty = 0;
    DUTcurrent = 0;
    DUTrpm = 0;
    Brake.setDuty(0);
    DUT.setDuty(0);
}

void Dyno::startDynoTest()
{
  if (testState_ != IDLE){return;}
  startTime_ = millis();
  testState_ = DYNOTEST;
  rpmSet = 10000.0;
  DUTduty = 0.95;
}

void Dyno::startTempTest()
{
  if (testState_ != IDLE){return;}
  startTime_ = millis();
  testState_ = TEMPTEST;

  //Zero average
  sumMeasurements_ = 0;
  numberMeasurements_ = 0;

  //500 rpm ensures that sensorless control will work
  rpm = 500.0;

  //PID settings
  PID.reset();
  PID.setPID(maxCurrent_/10.0, maxCurrent_/1000.0, 0.0);
  PID.setLimits(0.0, 100.0);
}

void Dyno::startPoleCheck()
{
  if (testState_ != IDLE){return;}
  startTime_ = millis();
  testState_ = POLECHECK;
}

void Dyno::dynoTest()
{
  ramp(rpmSet, 1, 100, rpm);
  Brake.setRPM(rpm*brakePoles);
  DUT.setDuty(DUTduty);

  //Stop criteria
  if (rpm == rpmSet || DUTinputCurrent < 0.0 || DUTdutyActual >= 0.95 || dutyActual >= 0.95)
  {
    stopTest();
  }
}

void Dyno::tempTest()
{
  //Spin up brake motor
  Brake.setRPM(rpm*brakePoles);

  //PID control current on DUT
  PID.pid(maxTemp_, DUTtemp, DUTcurrent);
  DUT.setCurrent(DUTcurrent);

  //Timed sequences, falling order:
  //Run test for 10 minutes, then stop
  if (millis() - startTime_ >= 600000)
  {
    stopTest();
  }
  //Average current the last minute is nominal current
  else if (millis() - startTime_ >= 600000-60000)
  {
    sumMeasurements_ += DUTmotorCurrent;
    numberMeasurements_++;
    DUTnominalCurrent_ = sumMeasurements_/numberMeasurements_;
  }
  //Find maximum current the DUT vesc is set to
  else if (millis() - startTime_ < 1000)
  {
    DUT.setCurrent(100.0);
    numberMeasurements_++;
    sumMeasurements_ += DUTmotorCurrent;
    maxCurrent_ = sumMeasurements_/numberMeasurements_;
  }
  else
  {
    //Zero average
    sumMeasurements_ = 0;
    numberMeasurements_ = 0;
    PID.setLimits(0.0, maxCurrent_);
  }

  //Stop if overtemp or reading lost
  //Counter to account for bad readings
  if (DUTtemp > 1.1*maxTemp_ || DUTtemp < 0.0)
  {
    counter++;
    if (counter >= 10)
    {
      counter = 10;
      stopTest();
    }
  }
  else counter = 0;
}

void Dyno::poleCheck()
{
  DUTrpmSet = 10000;
  DUT.setRPM(DUTrpmSet);
  if (millis() - startTime_ >= 3000)
  {
    polePairs_ = (int)round(DUTrpmSet/rpmActual);
    stopTest();
  }
}

void Dyno::update()
{
  //Get data from VESC's
  Brake.getVescValues();
  DUT.getVescValues();
  rpmActual = Brake.data.rpm/brakePoles;
  DUTrpmActual = DUT.data.rpm/polePairs_;
  motorCurrent = Brake.data.avgMotorCurrent;
  DUTmotorCurrent = DUT.data.avgMotorCurrent;
  dutyActual = Brake.data.dutyCycleNow;
  DUTdutyActual = DUT.data.dutyCycleNow;
  inputCurrent = Brake.data.avgInputCurrent;
  DUTinputCurrent = DUT.data.avgInputCurrent;
  inputVoltage = Brake.data.inpVoltage;
  DUTinputVoltage = DUT.data.inpVoltage;

  //Print log data
  printLog();
    
  //Update the current running program
  switch(testState_)
  {
    case IDLE:
      //Do nothing
      break;
    case DYNOTEST:
      dynoTest();
      break;
    case TEMPTEST:
      tempTest();
      break;
    case POLECHECK:
      poleCheck();
      break;
  }
}

int Dyno::getTestState()
{
  return testState_;
}

void Dyno::printLog()
{
  Serial.print(millis()-startTime_);
  Serial.print(',');
  Serial.print(testState_);
  Serial.print(',');
  Serial.print(abs(rpmActual),0);
  Serial.print(',');
  Serial.print(dutyActual);
  Serial.print(',');
  Serial.print(temp,1);
  Serial.print(',');
  Serial.print(motorCurrent);
  Serial.print(',');
  Serial.print(inputCurrent);
  Serial.print(',');
  Serial.print(inputVoltage);
  Serial.print(',');
  Serial.print(abs(DUTrpmActual),0);
  Serial.print(',');
  Serial.print(DUTdutyActual);
  Serial.print(',');
  Serial.print(DUTtemp,1);
  Serial.print(',');
  Serial.print(DUTmotorCurrent);
  Serial.print(',');
  Serial.print(DUTinputCurrent);
  Serial.print(',');
  Serial.print(DUTinputVoltage);
  Serial.print(',');
  Serial.print(polePairs_);
  Serial.print(',');
  Serial.print(DUTnominalCurrent_);
  Serial.print(',');
  Serial.print(lc0);
  Serial.print(',');
  Serial.print(lc1);
  Serial.print(',');
  Serial.print(lc2);
  Serial.print(',');
  Serial.print(lc3);
  Serial.print(',');
  Serial.print(abs(torque));
  Serial.print(',');
  Serial.print(abs(DUTtorque));
  Serial.print('\n');
}
