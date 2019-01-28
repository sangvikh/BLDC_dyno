#include "config.h"

CANbaud = 500000;
serialBaud = 115200;

//HX711 scale setup
CLK = 5;      // clock pin to the load cell amp
DOUTS[1] = {6};    //data pins
CHANNEL_COUNT = sizeof(DOUTS)/sizeof(byte);
