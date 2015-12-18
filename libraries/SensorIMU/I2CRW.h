/*
  I2CRW.h - Library for reading and writing data from/to I2C devices.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#ifndef I2CRW_h
#define I2CRW_h

#include "Arduino.h"

class I2CRW
{
  public:
    void writeRegister(int deviceAddress, byte address, byte val);
	int readRegister(int deviceAddress, byte address);
};

extern I2CRW I2C;

#endif