/*
  GY86.cpp - Library for working with GY86 IMU.
  Created by Rafael Araujo Lehmkuhl, May 09, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "GY86.h"
#include "I2CRW.h"
#include "IMU.h"
#include "SORT.h"
#include <Wire.h>

GY86 Device;

double Off_Xa, Off_Ya, Off_Za;    // Variáveis de offset para o acelerômetro
double Off_Xg, Off_Yg, Off_Zg;    // Variáveis de offset para o giroscópio

double divisorGYR;
double divisorACC;

double gXc, gYc, gZc;             // Variáveis para os três eixos do giroscópio
double aXc, aYc, aZc;             // Variáveis para os três eixos do acelerômetro
double tempCalibrada;


void GY86::atualizaValoresCalibrados(){
  Unit.atualizaValores();
  gXc = ((Unit.getGyXRaw() - Off_Xg)/divisorGYR)/57.3;
  gYc = ((Unit.getGyYRaw() - Off_Yg)/divisorGYR)/57.3;
  gZc = ((Unit.getGyZRaw() - Off_Zg)/divisorGYR)/57.3;

  aXc = (Unit.getAcXRaw() - Off_Xa)/divisorACC;
  aYc = (Unit.getAcYRaw() - Off_Ya)/divisorACC;
  aZc = (Unit.getAcZRaw() - Off_Za)/divisorACC;

  tempCalibrada = ((Unit.getTempRaw())/340.00+36.53);
}



double GY86::getAcXCalibrado(){
  return aXc;
}

double GY86::getAcYCalibrado(){
  return aYc;
}

double GY86::getAcZCalibrado(){
  return aZc;
}

double GY86::getGyXCalibrado(){
  return gXc;
}

double GY86::getGyYCalibrado(){
  return gYc;
}

double GY86::getGyZCalibrado(){
  return gZc;
}

double GY86::getTempCalibrada(){
  return tempCalibrada;
}






void GY86::calibrateGYR(int samples){
  Off_Xg=0;
  Off_Yg=0;
  Off_Zg=0;
  
  double X[samples];
  double Y[samples];
  double Z[samples];
    
  for(int i=0;i<samples;i++)
  {
      Unit.atualizaValores();
      X[i] = Unit.getGyXRaw();
      Y[i] = Unit.getGyYRaw();
      Z[i] = Unit.getGyZRaw();

    Off_Xg += X[i];
    Off_Yg += Y[i];
    Off_Zg += Z[i];
  }
  
  Off_Xg = Off_Xg/samples;
  Off_Yg = Off_Yg/samples;
  Off_Zg = Off_Zg/samples;
}

void GY86::calibrateACC(int samples){
  Off_Xa=0;
  Off_Ya=0;
  Off_Za=0;
  
  double X[samples];
  double Y[samples];
  double Z[samples];
    
  for(int i=0;i<samples;i++){
    Unit.atualizaValores();
    X[i] = Unit.getAcXRaw();
    Y[i] = Unit.getAcYRaw();
    Z[i] = Unit.getAcZRaw();

    Off_Xa += X[i];
    Off_Ya += Y[i];
    Off_Za += Z[i];
  }
  
  Off_Xa = Off_Xa/samples;
  Off_Ya = Off_Ya/samples;
  Off_Za = (Off_Za/samples)-divisorACC;
}




int GY86::setupGYR(int scale){
  I2C.writeRegister(GYR_Address, GYR_Register_1, 0b00000000);
  
  if(scale==250) 
    {
      I2C.writeRegister(GYR_Address, GYR_Register_2, 0b00000000);
      divisorGYR = 131;
    }
  if(scale==500) 
    {
      I2C.writeRegister(GYR_Address, GYR_Register_2, 0b00001000);
      divisorGYR = 65.5;
    }
  if(scale==1000) 
    {
      I2C.writeRegister(GYR_Address, GYR_Register_2, 0b00010000);
      divisorGYR = 32.8;
    }
  if(scale==2000) 
    {
      I2C.writeRegister(GYR_Address, GYR_Register_2, 0b00011000);
      divisorGYR = 16.4;
    } 
}

void GY86::setupACC(int scale){
  I2C.writeRegister(ACC_Address, ACC_Register_1, 0b00000000);  

  if(scale==2) 
    {
      I2C.writeRegister(ACC_Address, ACC_Register_2, 0b00000000);
      divisorACC = 16384;
    }
  if(scale==4) 
    {
      I2C.writeRegister(ACC_Address, ACC_Register_2, 0b00001000);
      divisorACC = 8192;
    }
  if(scale==8) 
    {
      I2C.writeRegister(ACC_Address, ACC_Register_2, 0b00010000);
      divisorACC = 4096;
    }
  if(scale==16) 
    {
      I2C.writeRegister(ACC_Address, ACC_Register_2, 0b00011000);
      divisorACC = 2048;
    } 
}




