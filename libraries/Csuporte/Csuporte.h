#ifndef Csuporte_h
#define Csuporte_h

#include "Arduino.h"

class Csuporte{
  public:
    int mediaAnteriores(int dataAnterior[]);
    void avancaTrilha(int* dataAnterior, int novaData);
    float mediaAnteriores(float dataAnterior[]);
    void avancaTrilha(float* dataAnterior, float novaData);
    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
};





#endif


