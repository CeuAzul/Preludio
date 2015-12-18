/*
  SORT.h - Library for sorting a set of 3 numbers.
  Created by Rafael Araujo Lehmkuhl, March 29, 2015.
  Released into the public domain.
*/

#ifndef SORT_h
#define SORT_h

#include "Arduino.h"

class SORT
{
  public:
	void sort(double a[]);
};

extern SORT SORTED;

#endif