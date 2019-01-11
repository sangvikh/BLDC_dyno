/* BLDC dyno
 * Controls two VESC's via CAN bus
 * 11" HMI
 */

//Includes
#include <SPI.h>
#include "mcp_can.h"

//Constants
const int spiPin = 10;
const int ledPin = 2;

//Initialize MCP
MCP_CAN CAN(spiPin);

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin,OUTPUT);

    while (CAN_OK != CAN.begin(CAN_250KBPS, MCP_8MHz))              // init can bus : baudrate = 250k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];

    //Read CAN messages
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        
        unsigned long canId = CAN.getCanId();

        Serial.println("-----------------------------");
        Serial.print("get data from ID: 0x");
        Serial.println(canId, HEX);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i]);
            Serial.print("\t");
        }
        Serial.println();
    }

    //Send CAN message
    char msg[8] = {1, 3, 3, 7};
    len = 4;
    CAN.sendMsgBuf(0x70, 1, len, msg);
    
    //Delay
    delay(1000);
}
