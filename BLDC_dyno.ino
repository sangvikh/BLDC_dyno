/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <FlexCAN.h>
#include "VescUart.h"
#include "Loadcell.h"
#include "Brake.h"
#include "config.h"
#include "functions.h"
#include "variables.h"

//Initiate classes
VescUart Brake;   //Brake VESC
VescUart DUT;     //Device under test VESC
CycleTime Main(10);  //Creates a check for a fixed cycle time
LoadCell Loadcell;   //Initiate scales

//Global variables
static CAN_message_t inMsg;
static CAN_message_t msg;
float rpmSet = 0.0;
float currentSet = 0.0;
float rpm = 0.0;
long loadCell = 0;


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

  //PinModes
  pinMode(13,OUTPUT);
}


void loop()
{  
  if(Main.checkTime()) //
  {
    //Gets the cycle time
    float cycleTime = Main.getCycleTime();
    
    //Get data from brake
    Brake.getVescValues();

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
      }
    }
    
    //RPM ramping
    ramp(rpmSet, 10.0, 10000.0, rpm, cycleTime);
    
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
    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 4;
    msg.buf[0] = 1;
    msg.buf[1] = 3;
    msg.buf[2] = 3;
    msg.buf[3] = 7;
    Can0.write(msg);

    //Set outputs
    

    //Print messages
    //Serial.println(loadCell);
    Serial.println(Main.getCycleTime());
  }
}
