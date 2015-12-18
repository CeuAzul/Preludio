#include <Csuporte.h>

//Biblioteca de suporte



void Csuporte::avancaTrilha(int* dataAnterior, int novaData){
 int numPros = sizeof(dataAnterior);
 for(int cont = 0; cont < numPros; cont++){
     if(cont == (numPros-1)){                      //Ultimo valor recebe o valor atual
       dataAnterior[cont] = novaData;
     }else{
       dataAnterior[cont] = dataAnterior[cont+1];  //Os outros recebem o seguintes a eles mesmos
     }
 } 
}

float Csuporte::mediaAnteriores(float dataAnterior[]){
  int numPros = sizeof(dataAnterior);
  float media = 0;                                      //Reseta a média
     for(int cont = 0; cont < numPros; cont++){
       media = media + dataAnterior[cont];
     }
  media = media/numPros;                          //Faz nova média 
  return media;
}

void Csuporte::avancaTrilha(float* dataAnterior, float novaData){
 int numPros = sizeof(dataAnterior);
 for(int cont = 0; cont < numPros; cont++){
     if(cont == (numPros-1)){                      //Ultimo valor recebe o valor atual
       dataAnterior[cont] = novaData;
     }else{
       dataAnterior[cont] = dataAnterior[cont+1];  //Os outros recebem o seguintes a eles mesmos
     }
 } 
}

int Csuporte::mediaAnteriores(int dataAnterior[]){
  int numPros = sizeof(dataAnterior);
  int media = 0;                                      //Reseta a média
     for(int cont = 0; cont < numPros; cont++){
       media = media + dataAnterior[cont];
     }
  media = media/numPros;                          //Faz nova média 
  return media;
}

float Csuporte::mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
