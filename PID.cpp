#include "PID.h"
#include "variables.h"
#include <Arduino.h>

PID::PID(){};
PID::~PID(){};

void PID::pid(float sp, float pv, float &out)
{
  //Calculate cycle time
  dt_ = (float)(micros()-lastTime_)/1000000.0;

  //Calculate error, integral and derivative
  error_ = sp - pv;
  integral_ += error_*dt_*antiWindup();
  derivative_ = (lastPv_ - pv)/dt;
  lastTime_ = micros();
  lastPv_ = pv;

  //Update output
  control_ = error_*kp_ + integral_ + derivative_*kd_;
  out_ = constrain(control_, 0.0, 50.0);
  out = out_;
}

void PID::setPID(float kp, float ki, float kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

float PID::getError()
{
  return error_;
}

float PID::getIntegral()
{
  return integral_;
}

float PID::getdt()
{
  return dt_;
}

void PID::reset()
{
  integral_ = 0;
}

float PID::antiWindup()
{
  if (control_ != out_)
  {
    return 0;
  }
  else
  {
    return ki_;
  }
}
