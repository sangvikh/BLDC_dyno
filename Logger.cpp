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

  //Open the log file
  logFile = SD.open(fileName_, FILE_WRITE);
}

void Logger::log(float *data)
{
  char dataString[32];
  sprintf(dataString, "%f, %f", data[0], data[1]);
  logFile.println(dataString);
}

void Logger::end()
{
  //Close log file
  logFile.close();
}

void Logger::setFileName(char input[])
{
  strcpy(fileName_, input);
}
