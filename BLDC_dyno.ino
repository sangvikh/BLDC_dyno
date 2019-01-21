/* BLDC dyno
 * Controls two VESC's via UART, controlled via CAN, SD card logging
 * 11" HMI
 */

//Includes
#include <VescUart.h>
#include <FlexCAN.h>

//Initiate classes
VescUart Brake;   //Brake VESC
//VescUart DUT;     //Device under test VESC
FlexCAN CAN(500000);

//Global variables
static CAN_message_t msg;

void setup() {

  //Setup debug serial
  Serial.begin(115200);

  //Setup serial to brake VESC
  Serial1.begin(115200);

  //Setup serial to DUT VESC
  //Serial2.begin(115200);

  //Define which serial ports to use
  Brake.setSerialPort(&Serial1);
  //DUT.setSerialPort(&Serial2);

  //Begin CAN communication
  CAN.begin();

  delay(100); //Wait until communication is initiated
}

void loop()
{  
  //Get data from brake
  Brake.getVescValues();
  Serial.print("RPM: "); Serial.println(Brake.data.rpm);
  Serial.print("A: "); Serial.println(Brake.data.avgMotorCurrent);
  Brake.setBrakeCurrent(1.0);
    
  delay(500);
}
