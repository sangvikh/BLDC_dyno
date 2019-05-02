/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <FlexCAN.h>
#include "LoadCell.h"
#include "CycleTime.h"
#include "Dyno.h"
#include <Adafruit_MAX31865.h>
#include "config.h"
#include "functions.h"
#include "variables.h"

//Initiate classes
CycleTime Main(10);  //Creates a check for a fixed cycle time
LoadCell LoadCell;   //Initiate scales
Dyno Dyno;           //Dyno program sequence
Adafruit_MAX31865 TS0(18);  //Temperature sensor
Adafruit_MAX31865 TS1(19);  //Temperature sensor

//CAN message
static CAN_message_t msg;

void setup()
{
  //Setup debug serial
  Serial.begin(serialBaud);

  //Begin Dyno program
  Dyno.begin();

  //Begin CAN communication
  Can0.begin(CANbaud);

  //Load loadcell calibration values
  LoadCell.loadCalibration();
  LoadCell.tare();

  //Start temperature sensors
  TS0.begin(MAX31865_2WIRE);
  TS1.begin(MAX31865_2WIRE);

  //Door switch
  pinMode(doorPin, INPUT_PULLUP);
}


void loop()
{  
  if(Main.checkTime())
  {
    //Gets the cycle time, stores it in a global variable
    cycleTime = Main.getCycleTime();

    //Read incoming CAN messages
    while (Can0.available()) 
    {
      Can0.read(msg); 
      switch(msg.id)
      {
        case 0x01:
          DUTrpm = (float)(msg.buf[0]*100);
          break;
        case 0x02:
          rpm = 0.0;
          DUTrpm = 0.0;
          current = 0.0;
          DUTcurrent = 0.0;
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
        {
          unsigned int mass = (msg.buf[0]<<8)+msg.buf[1];
          LoadCell.setCalibrationMass((float)mass/1000.0);
          Serial.println(mass/1000, 3);
          break;
        }
        case 0x101:
          DUTrpm = (float)(msg.buf[0]*100);
          break;
        case 0x102:
          DUTrpm = 0.0;
          break;
        case 0x111:
          Dyno.startPoleCheck();
          msg.id = 0x112;
          msg.len = 1;
          break;
        case 0x112:
          Dyno.startTempTest();
          break;
      }
    }

    //Read serial data
    while (Serial.available() > 0)
    {
      char func = Serial.read();
      int value = Serial.parseInt();
      
      //Run functions
      switch(func)
      {
        default:
          //Do nothing
          break;
        case '0':
          Dyno.stopTest();
          break;
        case 'a':
          Dyno.startPoleCheck();
          break;
        case 'b':
          LoadCell.tare();
          Dyno.startDynoTest();
          break;
        case 'c':
          Dyno.startTempTest();
          break;
        case 'd':
          LoadCell.tare();
          break;
        case 'e':
          LoadCell.zero(value);
          break;
        case 'f':
          LoadCell.span(value);
          break;
        case 'g':
          LoadCell.setCalibrationMass((float)value/1000.0);
          break;
        case 'h':
          LoadCell.saveCalibration();
          break;
      }
    }

    //Emergency stop if door is open
    if (digitalRead(doorPin))
    {
      Dyno.stopTest();
    }

    //Update status of program sequence
    Dyno.update();
    //Update load cells
    LoadCell.refresh();
    temp = TS0.temperature(RNOMINAL, RREF);
    DUTtemp = TS1.temperature(RNOMINAL, RREF);

    //Write load cell values
    lc0 = LoadCell.getScaledValue(0);
    lc1 = LoadCell.getScaledValue(1);
    lc2 = LoadCell.getScaledValue(2);
    lc3 = LoadCell.getScaledValue(3);
    torque = LoadCell.getTorque(0,1);
    DUTtorque = LoadCell.getTorque(2,3);

    //Test CAN, also useful for verifying cycle time in PCAN
    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 4;
    msg.buf[0] = 1;
    msg.buf[1] = 3;
    msg.buf[2] = 3;
    msg.buf[3] = 7;
    Can0.write(msg);
  }
}
