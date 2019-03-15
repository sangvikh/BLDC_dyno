#ifndef DYNO_H
#define DYNO_H

class Dyno
{
public:
  Dyno();
  ~Dyno();

  //Starts all the supporting functions
  void begin();

  //Ends the supporting functions
  void end();
  
  //Starts a dyno run
  void startDynoTest();

  //Stops the current dyno run
  void stopTest();

  //Starts a temperature check, finds maximum nominal current
  void startTempTest();

  //Finds the number of pole pairs on the DUT
  void startPoleCheck();

  //This function runs every loop and keeps track of the output/states
  void update();
  
private:
  void dynoTest();
  void tempTest();
  int  poleCheck();
  
  //Class variables
  bool dynoTest_ = 0;  //High when dyno run is active
  bool tempTest_ = 0;  //High when temperature test is active  
  unsigned long startTime_ = 0; //Variable to store time when test was started
};

#endif
