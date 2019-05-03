#include "Filter.h"

Filter::Filter(){}
Filter::~Filter(){}

long Filter::medianFilter(long in)
{
  //Store input values in a circular buffer
  filterBuffer_[bufferLoc_] = in;
  bufferLoc_++;
  if (bufferLoc_ >= bufferLength_-1){bufferLoc_ = 0;}

  //Copy the array into another array which will be sorted
  for (int i = 0; i < bufferLength_; i++)
  {
    sortedArray_[i] = filterBuffer_[i];
  }
  
  //Sort array
  isort(sortedArray_, bufferLength_);

  //Return middle value(Median)
  return sortedArray_[(bufferLength_ - 1) / 2];
}

void Filter::isort(long *a, int n)
{
 for (int i = 1; i < n; ++i)
 {
   long j = a[i];
   long k;
   for (k = i - 1; (k >= 0) && (j < a[k]); k--)
   {
     a[k + 1] = a[k];
   }
   a[k + 1] = j;
 }
}
