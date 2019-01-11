/* BLDC dyno
 * Controls two VESC's via CAN bus
 * 11" HMI
 */

//Includes
#include <SPI.h>
#include "mcp_can.h"

//Constants
const int spiPin = 10;
const int ledPin = 3;
const int intPin = 2;

//Initialize MCP
MCP_CAN CAN(spiPin);

//Global variables
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 8MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if(CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(intPin, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  //Read CAN messages
  if(!digitalRead(intPin))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  }

  //Send CAN message
  char msg[8] = {1, 3, 3, 7};
  int len = 4;
  CAN.sendMsgBuf(0x70, 1, len, msg);
  CAN.sendMsgBuf(0x71, 1, len, msg);
  delay(100);   // send data per 100ms
}
