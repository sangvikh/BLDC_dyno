#ifndef LoadCell_H
#define LoadCell_H

class LoadCell
{
public:
  LoadCell();
  ~LoadCell();

  //Zeroes specified load cell
  void zero();

  //Sets the span of specified load cell
  void span();

  //Tares all load cells
  void tare();

  //Updates values
  void refresh();
  
private:
  unsigned char numberOfLoadCells_;
  long zeroValue_[4];
  long spanValue_[4];
  long tareValue_[4];
  long rawValue_[4];
};

#endif
