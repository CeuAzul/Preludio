/*
  Header do sensor do Pwm
*/
#ifndef SensorPwm_h
#define SensorPwm_h

#include "Arduino.h"


class SensorPwm{
  public:
    SensorPwm(int canal1pin, int canal2pin, int canal3pin, int canal4pin);
    void atualizaPWM(long valor, int vez);             
    void setModoRPM(int pinoVRPM);      // CASO USE RPM, CHAME ESSA FUNÇÃO COM O PARAMETRO TRUE (por default ela é FALSE)
    void calibraPWM(int nPWM, int pwmZero, int pwmMaximo, int grauMaximo);

    //Getters----------------------------------------------
    float getGrauPWM1();
    float getGrauPWM2();
    float getGrauPWM3();
    long getGrauPWM4();
    void pegaAngulo1(long valor);
    void pegaAngulo2(long valor);
    void pegaAngulo3(long valor);
    void pegaAngulo4(long valor);
    float getRPM();
    float getTensaoMotor();
    float getPotenciaDeEntrada();
    float getCorrenteUsada();
    float getRawVoltage();
    float getPwm4Raw(); 
    float getPwm3Raw(); 
    float getPwm2Raw();   
    float getPwm1Raw(); 
};


#endif
