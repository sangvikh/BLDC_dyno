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
CycleTime Main;  //Creates a check for a fixed cycle time

//Global variables
static CAN_message_t inMsg;
static CAN_message_t msg;
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
  Can0.begin(500000);

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
    Serial.print("RPMset: "); Serial.println(rpm);
    Serial.print("A: "); Serial.println(Brake.data.avgMotorCurrent);

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
          rpmSet = 15000.0;
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

    //Test CAN
    msg.ext = 0;
    msg.id = 0x100;
    msg.len = 8;
    msg.buf[0] = 0;
    msg.buf[1] = 0;
    msg.buf[2] = 0;
    msg.buf[3] = 0;
    msg.buf[4] = 1;
    msg.buf[5] = 3;
    msg.buf[6] = 3;
    msg.buf[7] = 7;
    Can0.write(msg);
  }
}
