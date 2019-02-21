/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <FlexCAN.h>
#include "LoadCell.h"
#include "CycleTime.h"
#include "Dyno.h"
#include "config.h"
#include "functions.h"
#include "variables.h"

//Initiate classes
CycleTime Main(10);  //Creates a check for a fixed cycle time
LoadCell LoadCell;   //Initiate scales
Dyno Dyno;           //Dyno program sequence

//CAN message
static CAN_message_t msg;

void setup()
{
  //Setup debug serial
  Serial.begin(serialBaud);

  //Initialize Dyno class
  Dyno.begin();

  //Begin CAN communication
  Can0.begin(CANbaud);

  //Load loadcell calibration values
  LoadCell.loadCalibration();
}


void loop()
{  
  if(Main.checkTime())
  {
    //Gets the cycle time, stores it in a global variable
    cycleTime = Main.getCycleTime();    

    //Update load cells
    LoadCell.refresh();
    torque = LoadCell.getScaledValue(0);

    //Read incoming CAN messages
    while (Can0.available()) 
    {
      Can0.read(msg); 
      switch(msg.id)
      {
        case 0x21:
          digitalWrite(13,!digitalRead(13));    //Toggle LED
          break;
        case 0x01:
          rpm = (float)(msg.buf[0]*100);
          current = 3.0;
          break;
        case 0x02:
          rpm = 0.0;
          current = 0.0;
          break;
        case 0x99:
          Dyno.startDynoTest();
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
          LoadCell.setCalibrationMass((float)msg.buf[0]/10);
          break;
      }
    }

    //Update status of program sequence
    Dyno.update();
    
    //Test CAN, also useful for verifying cycle time in PCAN
    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 4;
    msg.buf[0] = 1;
    msg.buf[1] = 3;
    msg.buf[2] = 3;
    msg.buf[3] = 7;
    Can0.write(msg);

    //Print messages
    Serial.println(rpmActual);
  }
}
