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

//Counter to filter readings
int counter = 0;

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
      counter++;
      if (counter >= 10)
      {
        counter = 10;
        Dyno.stopTest();
      }
    }
    else counter = 0;
    
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

    //Update status of program sequence
    Dyno.update();

    //Write status to CAN
    msg.id = 0x007;
    msg.len = 1;
    msg.buf[0] = Dyno.getTestState();
    Can0.write(msg);
  }
}
