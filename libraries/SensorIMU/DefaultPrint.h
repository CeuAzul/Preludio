/*
  DefaultPrint.h - Library for printing data in a default way.
  Created by Rafael Araujo Lehmkuhl, April 11, 2015.
  Released into the public domain.
*/

#ifndef DefaultPrint_h
#define DefaultPrint_h

#include "Arduino.h"

class DefaultPrint
{
  public:
	void print_triple(double x, double y, double z);
	void print_single(double x);
};

extern DefaultPrint Data;

#endif