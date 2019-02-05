#include <SD.h>
#include <SPI.h>

//SPI CS pin
const int chipSelect = 10;

//Begin SD logger
Serial.print("Initializing SD card...");

// see if the card is present and can be initialized:
if (!SD.begin(chipSelect))
{
  Serial.println("Card failed, or not present");
  // don't do anything more:
  return;
}
Serial.println("card initialized.");

//Opens a new log file
File dataFile = SD.open("datalog.txt", FILE_WRITE);

//Checks if file is available
if (dataFile)
{
  dataFile.println(dataString);
  dataFile.close();
  // print to the serial port too:
  Serial.println(dataString);
}  
// if the file isn't open, pop up an error:
else
{
  Serial.println("error opening datalog.txt");
} 
