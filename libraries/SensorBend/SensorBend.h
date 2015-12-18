// BEND SENSOR

#ifndef SensorBend_h
#define SensorBend_h

#include "Arduino.h"


class SensorBend{
  public:
    SensorBend(int canalBend);
    void atualizaBend();
    void setProcessarDados(bool devoProcessar);
    void calibraComoMaximo(float grauMaximo);
    void calibraComoZero();
    void calibraComoMinimo(float grauMinimo);
    float getGrauBend();  
    float getGrauBendMedia();  
    float getGrauBendRaw(); 
    float getCalibrAnalogMax();
    float getCalibrAnalogMin();
    float getCalibrAnalogZero();


};


#endif
