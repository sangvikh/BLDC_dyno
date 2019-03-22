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

  //Returns the error
  float getError();

  //Returns the integral of the error
  float getIntegral();

  //Returns timestep
  float getdt();

  //Resets integral
  void reset();

private:
  //Returns 0 if output is saturated, ki_ else.
  float antiWindup();
  
  float kp_ = 1;
  float ki_ = 0;
  float kd_ = 0;
  float dt_ = 0.001;
  float error_ = 0;
  float integral_ = 0;
  float derivative_ = 0;
  unsigned long lastTime_ = 0;
  float lastPv_ = 0;
  float control_ = 0;   //temporary control signal
  float out_ = 0;       //temporary output signal (used in anti windup)
};

#endif
