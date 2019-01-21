/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <VescUart.h>
#include <FlexCAN.h>
#include "Brake.cpp"

//Initiate classes
VescUart Brake;   //Brake VESC
//VescUart DUT;     //Device under test VESC
FlexCAN CAN(500000);
CycleTime Main;  //Creates a check for a fixed cycle time

//Global variables
static CAN_message_t inMsg;
static CAN_message_t outMsg;
float rpmSet = 0.0;
float rpm = 0.0;

void setup()
{
  //Setup debug serial
  Serial.begin(115200);

  //Setup serial to VESC
  Serial1.begin(115200);
  //Serial2.begin(115200);

  //Define which serial ports to use
  Brake.setSerialPort(&Serial1);
  //DUT.setSerialPort(&Serial2);

  //Begin CAN communication
  CAN.begin();

  //PinModes
  pinMode(13,OUTPUT);
}


void loop()
{  
  if(Main.checkTime()) //
  {
    //Get data from brake
    Brake.getVescValues();
    Serial.print("RPM: "); Serial.println(Brake.data.rpm);
    Serial.print("A: "); Serial.println(Brake.data.avgMotorCurrent);

    //Read incoming CAN messages
    while (CAN.available()) 
    {
      CAN.read(inMsg); 
      switch(inMsg.id)
      {
        case 0x21:
          digitalWrite(13,!digitalRead(13));    //Toggle LED
          break;
        case 0x01:
          rpmSet = 4000.0;
          break;
        case 0x02:
          rpmSet = 0.0;
          break;
      }
    }
    //RPM ramping
    Main.ramp(rpmSet, 0.001, rpm);
    
    //Set motor RPM
    Brake.setRPM(rpm);
  }
}
