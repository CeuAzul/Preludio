/*
  Header do sensor de ultrassom
*/
#ifndef SensorUltrassom_h
#define SensorUltrassom_h

#include "Arduino.h"
#include "NewPingCA.h"
 

class SensorUltrassom{
  public:
    SensorUltrassom(int distanciaChao = 15, int margemErro = 3, int millisParaConfirmacao = 750, int maxDist = 50, int triggerPin = 5, int echoPin = 4);
    void atualizaUltrassom();
    long getTempoDeVoo();     
    long getContador();                               //Retorna tempo de voo
 	  void calibraAquiMesmo(int nMedia = 10);
  	void calibraDistancia(int dist);
 	  void setDistanciaMargemErro(int margemErro);
    int getDistancia();                                           //Retorna distancia do chão
    int getDistReferencia();
    bool getWoWRaw();
	  bool getWoWUltrassom();
    long getTempoDeConfirmacao();
    long getUltimoTempoEncostadoNoChao();                //Retorna o ultimo milis() que o avião estava no chão

  private:
    void taNoChaoDeBoas();
    void taNoArDeBoas();
};





#endif
