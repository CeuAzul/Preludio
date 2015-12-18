/*
  Header do sensor de ruído
*/
#ifndef SensorRuido_h
#define SensorRuido_h

#include "Arduino.h"

class SensorRuido{
  public:
    SensorRuido(int pino);
    void atualizaRuido();
    int getVolume();
    int getMediaVolume();
    int getDesvioPadrao();
    int getSomaDesvioPadrao();
    int getMediaDesvioPadrao();
    
  private:
    void fazSomaDp();
};





#endif
