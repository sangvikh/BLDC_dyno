


//Log to SD card        <<<< Put inside some function
//Save in buffer
if (bufferLocation < bufferSDsize)
{
  sprintf(bufferSD[bufferLocation], "%f,%ld,%f", LoadCell.getScaledValue(0), Brake.data.rpm, cycleTime);
  bufferLocation++;
}
else
{
  //Write to card
  logFile = SD.open("log.txt", FILE_WRITE);
  for (int i = 0; i < bufferSDsize; i++)
  {
    logFile.println(bufferSD[i]);
  }
  logFile.close();
  bufferLocation = 0;
}
