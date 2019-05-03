#ifndef FILTER_H
#define FILTER_H

class Filter
{
public:
  Filter();
  ~Filter();

  //Median filter function
  long medianFilter(long in);

private:
  //Private functions
  void isort(long *a, int n);
  
  //Private variables
  const static int bufferLength_ = 7;
  long filterBuffer_[bufferLength_] = {0};
  long sortedArray_[bufferLength_] = {0};
  int bufferLoc_ = 0;
};

#endif
