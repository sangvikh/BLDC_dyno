#include "Dyno.h"
#include "config.h"
#include "variables.h"
#include "functions.h"
//#include "Logger.h"
#include "arduino.h"
#include "VescUart.h"
#include "PID.h"

//Logger Logger;
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
    state_++;       //For testEverytning();
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
//    Logger.end();
}

void Dyno::emgStop()
{
  stopTest();
  testEverything_ = 0;
}

void Dyno::testEverything()
{
  testEverything_ = 1;
  state_ = 0;
}

void Dyno::startDynoTest()
{
  if (testState_ != IDLE && testEverything_ == 0){return;}
  startTime_ = millis();
  testState_ = DYNOTEST;
  char text[] = "DYNO.TXT";
//  Logger.setFileName(text);
//  Logger.begin();
  rpmSet = 10000.0;
  DUTduty = 0.25;
}

void Dyno::startTempTest()
{
  if (testState_ != IDLE && testEverything_ == 0){return;}
  startTime_ = millis();
  testState_ = TEMPTEST;

  //Zero average
  sumMeasurements_ = 0;
  numberMeasurements_ = 0;

  //Start logger
//  char text[] = "TEMP.TXT";
//  Logger.setFileName(text);
//  Logger.begin();

  //Max current
  maxCurrent_ = 50.0;
  rpm = 500.0;

  //PID settings
  PID.reset();
  PID.setPID(maxCurrent_/10.0, maxCurrent_/1000.0, 0.0);
  PID.setLimits(0.0, maxCurrent_);
}

void Dyno::startPoleCheck()
{
  if (testState_ != IDLE && testEverything_ == 0){return;}
  startTime_ = millis();
  testState_ = POLECHECK;
}

int Dyno::getPolePairs()
{
  return polePairs_;
}

void Dyno::dynoTest()
{
  startTime_ = millis();
//  float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual, DUTtemp};
//  unsigned int length = sizeof(logData)/sizeof(float);
//  Logger.log(logData, length);
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
  //Start logging
//  float logData[] = {rpmActual, torque, cycleTime, inputVoltage, inputCurrent, motorCurrent, dutyActual, DUTinputCurrent, DUTmotorCurrent, DUTdutyActual, DUTtemp};
//  unsigned int length = sizeof(logData)/sizeof(float);
//  Logger.log(logData, length);
  Brake.setRPM(rpm*brakePoles);

  //PID control current
  PID.pid(maxTemp_, DUTtemp, DUTcurrent);
  DUT.setCurrent(DUTcurrent);

  //Average current the last minute is nominal current
  if (millis() - startTime_ >= 600000-60000)
  {
    sumMeasurements_ += DUTmotorCurrent;
    numberMeasurements_++;
  }

  //Run test for 10 minutes
  if (millis() - startTime_ >= 600000)
  {
    DUTnominalCurrent_ = sumMeasurements_/numberMeasurements_;
    stopTest();
  }

  int counter = 0;
  //Stop if overtemp or reading lost
  while (DUTtemp > 1.1*maxTemp_ || DUTtemp < 0.0)
  {
    counter++;
    if (counter > 10)
    {
      stopTest();
      break;
    }
  }
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

void Dyno::everything()
{
  switch(state_)
  {
    default:
      break;
    case 0:
      startPoleCheck();
      state_++;
      break;
    case 2:
      delay(500);
      startDynoTest();
      state_++;
      break;
    case 4:
      delay(500);
      startTempTest();
      state_++;
      break;
    case 6:
      testEverything_ = 0;
      stopTest();
      break;
  }
}

void Dyno::pidRPM()
{
  PID.setPID(0.03, 0.12, 0.001875);
  PID.reset();
  PID.setFilter(10);
  testState_ = PIDRPM;
}

void Dyno::setMaxTemp(float maxTemp)
{
  maxTemp_ = maxTemp;
}

void Dyno::setMaxCurrent(float maxCurrent)
{
  maxCurrent_ = maxCurrent;
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
    case PIDRPM:
      PID.pid(1000.0, rpmActual, DUTcurrent);
      DUT.setCurrent(DUTcurrent);
      Serial.println(rpmActual);
      Serial.println(PID.getIntegral());
      Serial.println(PID.getdt(),4);
      Serial.println("--------------------");
  }

  //Running if checkEverything();
  if (testEverything_ == 1)
  {
    everything();
  }
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
