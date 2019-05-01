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

  //Stops the current test
  void stopTest();

  //Stops everything
  void emgStop();
  
  //Starts a dyno run
  void startDynoTest();

  //Starts a temperature check, finds maximum nominal current
  void startTempTest();

  //Finds the number of pole pairs on the DUT
  void startPoleCheck();

  //This function runs every loop and keeps track of the output/states
  void update();

  //Sets maximum DUT temp, used for finding nominal current
  void setMaxTemp(float maxTemp);
  
private:
  void dynoTest();
  void tempTest();
  void poleCheck();
  void printLog();

  enum TESTSTATE
  {
    IDLE = 0,
    POLECHECK,
    DYNOTEST,
    TEMPTEST,
  };
  
  //Class variables
  TESTSTATE testState_ = IDLE;
  unsigned long startTime_ = 0; //Variable to store time when test was started
  int polePairs_ = 0;   //Number of pole pairs
  float maxTemp_ = 100.0;       //Maximum temperature for finding nominal current
  float maxCurrent_ = 100.0;       //Maximum motor current (About 2-3x nominal current)
  float DUTnominalCurrent_ = 0;     //Nominal current found in the currentCheck
  float sumMeasurements_ = 0;    //Sum for averaging
  unsigned int numberMeasurements_ = 0;   //Number of measurements for averaging
};

#endif
