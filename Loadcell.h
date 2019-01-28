#ifndef LoadCell_H
#define LoadCell_H

class LoadCell
{
public:
  LoadCell();
  ~LoadCell();

  //Zeroes specified load cell
  void zero(unsigned char i);

  //Sets the span of specified load cell
  void span(unsigned char i);

  //Tares all load cells
  void tare();
  
  //Updates values
  void refresh();

  //Returns torque from one of the pairs
  float getTorque(unsigned char lc1, unsigned char lc2);

  //Returns scaled value from one of the load cells
  float getScaledValue(unsigned char i) {return scaledValue_[i];}

  //Returns raw value from one of the load cells
  float getRawValue(unsigned char i) {return rawValue_[i];}

  //Saves calibration data
  void saveCalibration();

  //Loads calibration data
  void loadCalibration();
  
private:
  unsigned char numberOfLoadCells_;
  long zeroValue_[4] = {98000};
  long spanValue_[4] = {500000};
  long tareValue_[4] = {0};
  long rawValue_[4] = {};
  float scaledValue_[4] = {};
  float radius_ = 0.055;  //Distance between load cells
  float torque_[2] = {};
  float calibrationMass_ = 5.0;

  //Private help functions
  //Scales raw values to calibration values
  void scaleValues();
};

#endif
