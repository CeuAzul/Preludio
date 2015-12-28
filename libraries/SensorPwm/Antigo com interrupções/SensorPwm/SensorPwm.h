/*
  Header do sensor do Pwm
*/
#ifndef SensorPwm_h
#define SensorPwm_h

#include "Arduino.h"


class SensorPwm{
  public:
    SensorPwm(int canal1pin, int canal2pin, int canal3pin, int canal4pin);
    void atualizaPWM();             
    void setModoRPM(int pinoVRPM);      // CASO USE RPM, CHAME ESSA FUNÇÃO COM O PARAMETRO TRUE (por default ela é FALSE)
    void calibraPWM(int nPWM, int pwmZero, int pwmMaximo, int grauMaximo);
    //Interrupções para leitura do timer =================
    void sinalCH1();
    void sinalCH2();
    void sinalCH3();
    void sinalCH4();
    void calcSignalCh1();
    void calcSignalCh2();
    void calcSignalCh3();
    void calcSignalCh4();

    //Getters----------------------------------------------
    float getGrauPWM1();
    float getGrauPWM2();
    float getGrauPWM3();
    float getGrauPWM4();
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
