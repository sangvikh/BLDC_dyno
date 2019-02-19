#include "Logger.h"
#include <SD.h>
#include <SPI.h>
#include "config.h"
#include "functions.h"
#include "variables.h"

File logFile;

Logger::Logger(){}
Logger::~Logger(){}

void Logger::begin()
{
  //set PinModes
  pinMode(13,OUTPUT);     //Not sure if needed
  pinMode(CSpin, OUTPUT);

  //Begin SD card
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  }
  else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

void Logger::log(float *data)
{
  logFile = SD.open(fileName_, FILE_WRITE);
  int length = sizeof(data)/sizeof(float);
  for (int i = 0; i < length; i++)
  {
    logFile.print(data[i]); logFile.print(",");
  }
  logFile.println();
}

void Logger::end()
{
  logFile.close();
}

void Logger::setFileName(char input[])
{
  strcpy(fileName_, input);
}
