#include "PID.h"
#include "variables.h"
#include <Arduino.h>

PID::PID(){}
PID::~PID(){}

void PID::pid(float sp, float pv, float &out)
{
  //Calculate cycle time
  time_ = micros();
  dt_ = (float)(time_-prevTime_)/1000000.0;

  //Calculate error, integral and derivative
  error_ = sp - pv;
  integral_ += error_*dt_*antiWindup();
  derivative_ = filter(error_ - prevError_)/dt_;

  //Store previous values
  prevTime_ = time_;
  prevError_ = error_;

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

void PID::setFilter(unsigned int beta)
{
  filterBeta_ = beta;
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
  prevTime_ = micros();
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
  // LPF: Y(n) = (1-ß)*Y(n-1) + (ß*X(n))) = Y(n-1) - (ß*(Y(n-1)-X(n)));
  filteredValue_ = filteredValue_ - (filterBeta_ * (filteredValue_ - in));
  return filteredValue_;
}
