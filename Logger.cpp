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
  if (SD.begin(CSpin))
  {
    Serial.println("Logging started");
  }
  else
  {
    Serial.println("SD card initialization failed");
  }

  //Delete old file and open a new one
  SD.remove(fileName_);
  logFile = SD.open(fileName_, FILE_WRITE);
}

void Logger::log(float *data, unsigned int length)
{
  for (unsigned int i = 0; i < length; i++)
  {
    logFile.print(data[i], 3); logFile.print(',');
  }
  logFile.println();
}

void Logger::end()
{
  //Close log file
  logFile.close();
  Serial.println("Logging ended");
}

void Logger::setFileName(char input[])
{
  strcpy(fileName_, input);
}
