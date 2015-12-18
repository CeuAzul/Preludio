/*
  GY86.h - Library for working with GY86 IMU.
  Created by Rafael Araujo Lehmkuhl, May 09, 2015.
  Released into the public domain.
*/

#ifndef GY86_h
#define GY86_h

#include "Arduino.h"

//////////////////////////////GIROSCOPIO//////////////////////////////
#define GYR_Register_1 0x6B
#define GYR_Register_2 0x1B

#define GYR_Register_Xm 0x43
#define GYR_Register_Xl 0x44
#define GYR_Register_Ym 0x45
#define GYR_Register_Yl 0x46
#define GYR_Register_Zm 0x47
#define GYR_Register_Zl 0x48

#define GYR_Address 0X68    //Endereco I2C do MPU6050

/////////////////////////////ACELEROMETRO/////////////////////////////
#define ACC_Register_1 0x6B
#define ACC_Register_2 0x1C

#define ACC_Register_Xm 0x3B
#define ACC_Register_Xl 0x3C
#define ACC_Register_Ym 0x3D
#define ACC_Register_Yl 0x3E
#define ACC_Register_Zm 0x3F
#define ACC_Register_Zl 0x40

#define ACC_Address 0X68    //Endereco I2C do MPU6050

/////////////////////////////MAGNETOMETRO/////////////////////////////
#define MAG_Register_MODE 0x02

#define MAG_Register_Xm 0x03
#define MAG_Register_Xl 0x04
#define MAG_Register_Ym 0x07
#define MAG_Register_Yl 0x08
#define MAG_Register_Zm 0x05
#define MAG_Register_Zl 0x06

#define MAG_Address 30    // Define o endereco do HMC5883 - 0x1E ou 30 em decimal

class GY86
{
  public:
	void calibrateGYR(int samples);
	void calibrateACC(int samples);

	void atualizaValoresCalibrados();
	double getAcXCalibrado();
	double getAcYCalibrado();
	double getAcZCalibrado();
	double getGyXCalibrado();
	double getGyYCalibrado();
	double getGyZCalibrado();
		double getTempCalibrada();

	int  setupGYR(int scale);
	void setupACC(int scale);
};

extern GY86 Device;

#endif