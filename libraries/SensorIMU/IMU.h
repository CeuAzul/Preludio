/*
  IMU.h - Library for getting data from Inertial Measurement Unities.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#ifndef IMU_h
#define IMU_h

#include "Arduino.h"

class IMU
{
  public:
  	void atualizaValores();
    void inicializaIMURaw(int numMediasFiltro = 1, bool mpu400hz = true);
	double getAcXRaw();
	double getAcYRaw();
	double getAcZRaw();
	double getGyXRaw();
	double getGyYRaw();
	double getGyZRaw();
	double getTempRaw();
	
};

extern IMU Unit;

#endif