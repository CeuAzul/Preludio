/*
  DCMPI.h - Library for PI controlling DCM matrices.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#ifndef DCMPI_h
#define DCMPI_h

#include "Arduino.h"

class DCMPI
{
  public:
    void PI_controller(double DCM[3][3], double YcoG, double acc_vector[3], double dt, double W_Ico[3], double W_co[3], double W_RP, double W_Y, double K_P, double K_I);
};

extern DCMPI DCMinstance;

#endif