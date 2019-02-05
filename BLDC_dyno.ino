/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
//#include <FlexCAN.h>
#include "VescUart.h"
#include "LoadCell.h"
#include "CycleTime.h"
#include "config.h"
#include "functions.h"
#include "variables.h"
#include <SD.h>
#include <SPI.h>

//Initiate classes
VescUart Brake;   //Brake VESC
VescUart DUT;     //Device under test VESC
CycleTime Main(10);  //Creates a check for a fixed cycle time
LoadCell LoadCell;   //Initiate scales

//Global variables
//static CAN_message_t inMsg;
//static CAN_message_t msg;
float rpmSet = 0.0;
float currentSet = 0.0;
float rpm = 0.0;
File logFile;
const int bufferSDsize = 50;
char bufferSD[bufferSDsize][32];
int bufferLocation = 0;


void setup()
{
  //Setup debug serial
  Serial.begin(serialBaud);
  Serial.setDebugOutput(true);

  //Setup serial to VESC's
  Serial1.begin(serialBaud);
  Serial2.begin(serialBaud);

  //Define which serial ports to use for VESC's
  Brake.setSerialPort(&Serial1);
  DUT.setSerialPort(&Serial2);

  //Begin CAN communication
//  Can0.begin(CANbaud);

  //PinModes
  pinMode(13,OUTPUT);
  pinMode(CSpin, OUTPUT);

  //Load loadcell calibration values
  LoadCell.loadCalibration();

  //Begin SD card
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  }
  else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}


void loop()
{  
  if(Main.checkTime()) //
  {
    //Gets the cycle time
    float cycleTime = Main.getCycleTime();
    
    //Get data from brake
    Brake.getVescValues();

    //Update load cells
    LoadCell.refresh();
/*
    //Read incoming CAN messages
    while (Can0.available()) 
    {
      Can0.read(inMsg); 
      switch(inMsg.id)
      {
        case 0x21:
          digitalWrite(13,!digitalRead(13));    //Toggle LED
          break;
        case 0x01:
          rpmSet = (float)(inMsg.buf[0]*100);
          currentSet = 3.0;
          break;
        case 0x02:
          rpmSet = 0.0;
          currentSet = 0.0;
          break;
        case 0x99:
          Main.setCycleTime(inMsg.buf[0]);
          break;
        case 0x10:
          LoadCell.zero(0);
          break;
        case 0x11:
          LoadCell.span(0);
          break;
        case 0x12:
          LoadCell.tare();
          break;
        case 0x05:
          LoadCell.saveCalibration();
          break;
        case 0x06:
          LoadCell.setCalibrationMass((float)inMsg.buf[0]/10);
          break;
      }
    }
*/    
    //RPM ramping
    ramp(rpmSet, 10.0, 10000.0, cycleTime, rpm);
    
    //Set motor RPM and current
    if (Brake.data.rpm >= rpmSet*0.95 && Brake.data.avgMotorCurrent < currentSet)
    {
      Brake.setRPM(rpm);
    }
    else
    {
      Brake.setCurrent(currentSet);
    }

    //Test CAN, also useful for verifying cycle time in PCAN
/*    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 4;
    msg.buf[0] = 1;
    msg.buf[1] = 3;
    msg.buf[2] = 3;
    msg.buf[3] = 7;
    Can0.write(msg);
*/
    //Set outputs

    //Print messages
    Serial.println(LoadCell.getScaledValue(0));

    ////////////////Log to SD card///////////////////////

    //Save in buffer
    if (bufferLocation < bufferSDsize)
    {
      sprintf(bufferSD[bufferLocation], "%f,%ld,%f", LoadCell.getScaledValue(0), Brake.data.rpm, cycleTime);
      bufferLocation++;
    }
    else
    {
      //Write to card
      logFile = SD.open("log.txt", FILE_WRITE);
      for (int i = 0; i < bufferSDsize; i++)
      {
        logFile.println(bufferSD[i]);
      }
      logFile.close();
      bufferLocation = 0;
    }
  }
}
