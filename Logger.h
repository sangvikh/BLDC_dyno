#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
public:
  Logger();
  ~Logger();

  //opens new file on SD card
  void begin();

  //Logs data to card, takes in a float array
  void log(float *data, unsigned int length);

  //Closes SD file
  void end();

  //Sets the filename of the log file
  void setFileName(char input[]);
  
private:
  char fileName_[16] = "log.txt";
};

#endif
