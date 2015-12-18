/*
  I2CRW.cpp - Library for reading and writing data from/to I2C devices.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "I2CRW.h"
#include <Wire.h>
  
I2CRW I2C;

void I2CRW::writeRegister(int deviceAddress, byte address, byte val) 
{
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int I2CRW::readRegister(int deviceAddress, byte address)
{
    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte
    while(!Wire.available()) 
    {
        // waiting
    }
    v = Wire.read();
    return v;
}