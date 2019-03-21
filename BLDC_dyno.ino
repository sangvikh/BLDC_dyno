/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <FlexCAN.h>
#include "VescUart.h"
#include "LoadCell.h"
#include "CycleTime.h"
#include "Dyno.h"
#include <Adafruit_MAX31865.h>
#include "config.h"
#include "functions.h"
#include "variables.h"

//Initiate classes
VescUart Brake;   //Brake VESC
VescUart DUT;     //Device under test VESC
CycleTime Main(10);  //Creates a check for a fixed cycle time
LoadCell LoadCell;   //Initiate scales
Dyno Dyno;           //Dyno program sequence
Adafruit_MAX31865 TS0(17);

//CAN message
static CAN_message_t msg;

void setup()
{
  //Setup debug serial
  Serial.begin(serialBaud);

  //Setup serial to VESC's
  Serial1.begin(serialBaud);
  Serial2.begin(serialBaud);

  //Define which serial ports to use for VESC's
  Brake.setSerialPort(&Serial1);
  DUT.setSerialPort(&Serial2);

  //Begin CAN communication
  Can0.begin(CANbaud);

  //Load loadcell calibration values
  LoadCell.loadCalibration();

  //Start temperature sensors
  TS0.begin(MAX31865_2WIRE);
}


void loop()
{  
  if(Main.checkTime())
  {
    //Gets the cycle time, stores it in a global variable
    cycleTime = Main.getCycleTime();
    
    //Get data from VESC's
    Brake.getVescValues();
    DUT.getVescValues();
    rpmActual = Brake.data.rpm/poleCount;
    torque = LoadCell.getTorque(0,1);
    motorCurrent = Brake.data.avgMotorCurrent;
    DUTmotorCurrent = DUT.data.avgMotorCurrent;
    dutyActual = Brake.data.dutyCycleNow;
    DUTdutyActual = DUT.data.dutyCycleNow;
    inputCurrent = Brake.data.avgInputCurrent;
    DUTinputCurrent = DUT.data.avgInputCurrent;
    inputVoltage = Brake.data.inpVoltage;
    DUTinputVoltage = DUT.data.inpVoltage;
    
    //Update load cells
    LoadCell.refresh();

    //Read incoming CAN messages
    while (Can0.available()) 
    {
      Can0.read(msg); 
      switch(msg.id)
      {
        case 0x01:
          rpm = (float)(msg.buf[0]*100);
          break;
        case 0x02:
          rpm = 0.0;
          current = 0.0;
          break;
        case 0x99:
          LoadCell.tare();
          Dyno.startDynoTest();
          break;
        case 0x100:
          Dyno.stopTest();
        case 0x10:
          LoadCell.zero(msg.buf[0]);
          break;
        case 0x11:
          LoadCell.span(msg.buf[0]);
          break;
        case 0x12:
          LoadCell.tare();
          break;
        case 0x05:
          LoadCell.saveCalibration();
          break;
        case 0x06:
          LoadCell.setCalibrationMass((float)msg.buf[0]/10);
          break;
        case 0x101:
          DUTrpm = (float)(msg.buf[0]*100);
          break;
        case 0x102:
          DUTrpm = 0.0;
          break;
        case 0x111:
          int polePairs = Dyno.poleCheck();
          msg.id = 0x112;
          msg.buf[0] = polePairs;
          Can0.write(msg);
      }
    }

    //Update status of program sequence
    Dyno.update();
    
    //Set VESC ouputs, set to 0 to disable writing.
    if (rpm > 0)
    {      
      Brake.setRPM(rpm*poleCount);
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
      DUT.setRPM(DUTrpm*poleCount);
    }
    if (DUTcurrent > 0)
    {      
      DUT.setCurrent(DUTcurrent);
    }
    if (DUTduty > 0)
    {      
      DUT.setDuty(DUTduty);
    }

    //Test CAN, also useful for verifying cycle time in PCAN
    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 4;
    msg.buf[0] = 1;
    msg.buf[1] = 3;
    msg.buf[2] = 3;
    msg.buf[3] = 7;
    Can0.write(msg);

    //Write debug data to serial
    //Serial.println(torque,4);
    //Serial.print(LoadCell.getScaledValue(0),4), Serial.print(", "); Serial.println(LoadCell.getScaledValue(1),4);
    Serial.print("Temperature = "); Serial.println(TS0.temperature(RNOMINAL, RREF));
  }
}
