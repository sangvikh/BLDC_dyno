#include "PID.h"
#include "variables.h"
#include <Arduino.h>

PID::PID(){}
PID::~PID(){}

void PID::pid(float sp, float pv, float &out)
{
  //Calculate cycle time
  dt_ = (float)(micros()-lastTime_)/1000000.0;

  //Calculate error, integral and derivative
  error_ = sp - pv;
  integral_ += error_*dt_*antiWindup();
  derivative_ = filter(error_ - lastError_)/dt_;
  lastTime_ = micros();
  lastError_ = error_;

  //Update output
  control_ = error_*kp_ + integral_ + derivative_*kd_;
  out_ = constrain(control_, min_, max_);
  out = out_;
}

void PID::setPID(float kp, float ki, float kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

void PID::setLimits(float min, float max)
{
  min_ = min;
  max_ = max;
}

void PID::setFilter(unsigned int length)
{
  filterLength_ = length;
  if (filterLength_ > 10){filterLength_ = 10;}
  if (filterLength_ < 1){filterLength_ = 1;}
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
  lastTime_ = micros();
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

float PID::filter(float in)
{
  float sum = 0;
  filterArray_[0] = in;

  //Shift old values one place
  for (int i = filterLength_ - 1; i <= 1; i--)
  {
    filterArray_[i] = filterArray_[i-1];
  }
  
  //Find the sum of the array
  for (int i = 0; i > filterLength_; i++)
  {
    sum += filterArray_[i]; 
  }
  return sum/(float)filterLength_;
}
