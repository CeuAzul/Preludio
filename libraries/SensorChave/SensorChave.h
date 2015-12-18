#ifndef SensorChave_h
#define SensorChave_h
#include "Arduino.h"

class SensorChave{
  public:
    SensorChave(int millisParaConfirmacao = 200, int pino = 49);
    void atualizaChave();
    int getValorDaChave();
    long getContadorChave();
    long getUltimoTempoEncostadoNoChaoChave();
    long getTempoDeConfirmacao();
    long getTempoDeVooChave();
    bool getWOW();

  private:
    void taNoChaoDeBoas();
    void taNoArDeBoas();

    
};


#endif
