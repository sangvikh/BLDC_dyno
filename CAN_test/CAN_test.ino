// demo: CAN-BUS Shield, receive data with interrupt mode
// when in interrupt mode, the data coming can't be too fast, must >20ms, or else you can use check mode
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


//Global variables
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
bool flagRecv = 0;
char msgString[128];                        // Array to store serial string



void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
}

void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index) {
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
        buffer[(*index)++] = number >> 24;
        buffer[(*index)++] = number >> 16;
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
}

void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index) {
        buffer[(*index)++] = number >> 24;
        buffer[(*index)++] = number >> 16;
        buffer[(*index)++] = number >> 8;
        buffer[(*index)++] = number;
}

// CAN commands
typedef enum {
        CAN_PACKET_SET_DUTY = 0,
        CAN_PACKET_SET_CURRENT,
        CAN_PACKET_SET_CURRENT_BRAKE,
        CAN_PACKET_SET_RPM,
        CAN_PACKET_SET_POS,
        CAN_PACKET_FILL_RX_BUFFER,
        CAN_PACKET_FILL_RX_BUFFER_LONG,
        CAN_PACKET_PROCESS_RX_BUFFER,
        CAN_PACKET_PROCESS_SHORT_BUFFER,
        CAN_PACKET_STATUS
} CAN_PACKET_ID;



void comm_can_set_rpm(uint8_t controller_id, float rpm) {
        int32_t send_index = 0;
        uint8_t buffer[4];
        buffer_append_int32(buffer, (int32_t)rpm, &send_index);

        CAN.sendMsgBuf(controller_id | ((uint32_t)CAN_PACKET_SET_RPM << 8), 1, send_index, buffer);
//        comm_can_transmit(controller_id | ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
}


void setup()
{
  
    Serial.begin(115200);

START_INIT:

    if(CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }

    attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
    
      comm_can_set_rpm(1,4000);
      comm_can_set_rpm(5,4000);
      delay(1000);

      comm_can_set_rpm(1,0000);
      comm_can_set_rpm(5,0000);
    delay(1000);

     comm_can_set_rpm(1,-4000);
      comm_can_set_rpm(5,-4000);

    delay(1000);
      comm_can_set_rpm(1,0000);
      comm_can_set_rpm(5,0000);

}

void MCP2515_ISR()
{
    flagRecv = 1;
}

void loop()
{
  
    //Read CAN messages
  if(!digitalRead(2))                         // If CAN0_INT pin is low, read receive buffer
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
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
