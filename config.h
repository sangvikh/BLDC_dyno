#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Baudrates
#define CANbaud 500000
#define serialBaud 115200

//HX711 scale setup
byte CLK;      // clock pin to the load cell amp
byte DOUTS[4];    //data pins
byte CHANNEL_COUNT;

#endif
