/*
  IMU.cpp - Library for getting data from Inertial Measurement Unities.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "IMU.h"
#include "I2CRW.h"
#include <Wire.h>
const int MPU=0x68;  // I2C address of the MPU-6050

IMU Unit;

namespace variaveisIMURaw{

double acX;
double acY;
double acZ;
double gyX;
double gyY;
double gyZ;
double temp;
int nMedias;

}

using namespace variaveisIMURaw;




void IMU::atualizaValores(){
  for (int i = 0; i < nMedias; i++){
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
    acX += Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    acY += Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    acZ += Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    temp += Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    gyX += Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    gyY += Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    gyZ += Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  }
  acX = acX/nMedias;
  acY = acY/nMedias;
  acZ = acZ/nMedias;
  gyX = gyX/nMedias;
  gyY = gyY/nMedias;
  gyZ = gyZ/nMedias;
  temp = temp/nMedias;
}

void IMU::inicializaIMURaw(int numMediasFiltro, bool mpu400hz){
  Wire.begin();
  if(mpu400hz){
    TWBR = 12;
  }
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  nMedias = numMediasFiltro;
}

double IMU::getAcXRaw(){
  return acX;
}

double IMU::getAcYRaw(){
  return acY;
}

double IMU::getAcZRaw(){
  return acZ;
}

double IMU::getGyXRaw(){
  return gyX;
}

double IMU::getGyYRaw(){
  return gyY;
}

double IMU::getGyZRaw(){
  return gyZ;
}

double IMU::getTempRaw(){
  return temp;
}
