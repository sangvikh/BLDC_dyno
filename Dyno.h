#ifndef BRAKE_H
#define BRAKE_H

class Dyno
{
public:
  Dyno();
  ~Dyno();
  
  //Starts a dyno run
  void startDynoTest();

  //Starts a temperature check, finds maximum nominal current
  void startTempTest();

  //This function runs every loop and keeps track of the output/states
  void update();
  
private:
  void dynoTest();
  void tempTest();
  
  //Class variables
  bool dynoTest_ = 0;  //High when dyno run is active
  bool tempTest_ = 0;  //High when temperature test is active  
};

#endif
