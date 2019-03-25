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

  //Stops the current test
  void stopTest();

  //Starts a temperature check, finds maximum nominal current
  void startTempTest();

  //Finds the number of pole pairs on the DUT
  void startPoleCheck();

  //Returns the pole pairs found from the poleCheck
  int getPolePairs();

  //This function runs every loop and keeps track of the output/states
  void update();

  //Controls RPM using current and PID
  void pidRPM();
  
private:
  void dynoTest();
  void tempTest();
  void poleCheck();
  
  //Class variables
  int testState_ = 0;  //0 - Idle, 1 - Dyno test, 2 Temperature test, 3 - Pole test
  unsigned long startTime_ = 0; //Variable to store time when test was started
  int polePairs_ = 0;   //Number of pole pairs
  float maxTemp_ = 70.0;       //Maximum temperature for finding nominal current
  float maxCurrent_ = 50.0;       //Maximum motor current (About 2-3x nominal current)
  float DUTnominalCurrent_ = 0;     //Nominal current found in the currentCheck
  float sumMeasurements_ = 0;    //Sum for averaging
  unsigned int numberMeasurements_ = 0;   //Number of measurements for averaging
};

#endif
