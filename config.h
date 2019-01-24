//Baudrates
#define CANbaud 500000
#define serialBaud 115200

//HX711 scale setup
#define CLK 5      // clock pin to the load cell amp
byte DOUTS[1] = {6};    //data pins
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
