#ifndef CYCLETIME_H
#define CYCLETIME_H

class CycleTime
{
public:
  CycleTime(unsigned long cycleTime = 10);
  ~CycleTime();

  //Sets the cycle time in milliseconds
  void setCycleTime(unsigned long cycleTime);

  //Returns the actual cycle time in seconds
  float getCycleTime();
  
  //Returns true if cycle time has passed
  //Only run once per loop
  bool checkTime();
  
private:
  unsigned long lastTime_;  //Last timestamp
  unsigned long cycleTime_; //Cycle time in microseconds
  unsigned long cycleTimeActual_; //Actual cycle time
};

#endif
