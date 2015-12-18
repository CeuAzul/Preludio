/*
  Header das funções do Pitot
*/
#ifndef SensorPitot_h
#define SensorPitot_h
#include "Arduino.h"

class SensorPitot{
  public:
    SensorPitot(int nSamples = 5, int pinPitotSolo = A7, int pinPessaoDinamica = A4, int pinAtaqueUP = A2, int pinAtaqueDOWN = A3, int pinBankRIGHT = A0, int pinBankLEFT = A1);
    void atualizaPitot(double densAr = 1.218);
    void calibraPitot(int nSamples);
    double getRawFrontal();
    double getRawCima();
    double getRawBaixo();
    double getRawEsquerda();
    double getRawDireita();
    double getRawVertical();
    double getRawHorizontal();
    double getRawPitotSolo();
    double getPressaoPitot();
    double getVelocidadeCalibrada();

    
};


#endif
