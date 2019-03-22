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
  integral_ += error_*dt_;
  derivative_ = lastPv_ - pv;
  lastTime_ = micros();
  lastPv_ = pv;

  //Anti windup
  antiWindup();

  //Update output
  out = error_*kp_ + integral_*ki_ + derivative_*kd_;
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

void PID::antiWindup()
{
  if (integral_ > 1000.0)
  {
    integral_ = 1000.0;
  }
  if (integral_ < -1000.0)
  {
    integral_ = -1000.0;
  }
}
