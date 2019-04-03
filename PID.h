#ifndef PID_H
#define PID_H

class PID
{
public:
  PID();
  ~PID();

  //Updates PID regulator
  void pid(float sp, float pv, float &out);

  //Sets PID parameters
  void setPID(float kp = 1, float ki = 0, float kd = 0);

  //Set output limits
  void setLimits(float min, float max);

  //Set derivative filter length
  void setFilter(unsigned int length);

  //Returns the error
  float getError();

  //Returns the integral of the error
  float getIntegral();

  //Returns timestep
  float getdt();

  //Resets integral
  void reset();

private:
  //Private member functions
  float antiWindup();
  float filter(float in);

  //Member variables
  float kp_ = 1;
  float ki_ = 0;
  float kd_ = 0;
  float dt_ = 0.001;
  float error_ = 0;
  float prevError_ = 0;
  float integral_ = 0;
  float derivative_ = 0;
  unsigned long time_ = 0;
  unsigned long prevTime_ = 0;
  float control_ = 0;   //temporary control signal
  float out_ = 0;       //temporary output signal (used in anti windup)
  float max_ = 100.0;
  float min_ = 0.0;
  int filterBeta_ = 3;
  float filteredValue_ = 0;
};

#endif
