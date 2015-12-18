/*
  SensorRuido - Biblioteca de funções relacionada ao sensor de ruído da plataforma de Telemetria Céu Azul
*/

#include "Arduino.h"
#include "SensorRuido.h"
#include <Csuporte.h>
using namespace std;

namespace variaveisRuido{
  int volumeA;
  int analogPin;
  const int numPros = 100;              //Número de valores anteriores a serem analizados
  const int numDp = 60;                 //Número de desvios padrões a serem levados em contas na somatória
  int dataAnterior[numPros];            //Array onde os valores anteriores estão estocados
  int dpAnterior[numDp];
  int passo = 0;                        //Variável vai cuidar dos N primeiros valores
  int passoDp = 0;                      //Mesma coisa que o de cima, mas para o Desvio Padrão
  int media = 0;                        //Média dos N ultimos valores
  int mediaDp = 0;
  int dP;                               //Valor do desvio padrão de cada valor em relação a média dos N valores anteriores
  int somDp;
  
  Csuporte sup;
}

using namespace variaveisRuido;


SensorRuido::SensorRuido(int pino) {                
   pinMode(pino, INPUT);
   analogPin = pino;
}


void SensorRuido::atualizaRuido() {
  
  volumeA = analogRead(analogPin);
  
   if(passo < numPros){                                   //Esse if coleta os N primeiros valores (para encher o array a primeira vez)
     dataAnterior[passo] = volumeA;
     passo++;
   }else{                                                 //Se o array já ta cheio, partiu fazer a análise dos dados
     sup.avancaTrilha(dataAnterior, volumeA);             //Último valor vira o penúltimo, o penultimo vira o ante-penúltimo, e por aí vai
     media = sup.mediaAnteriores(dataAnterior);           //Atualiza o int 'media' com o novo valor de média
     dP = abs(media - volumeA);                           //Pega o desvio padrão do ultimo valor, em relação a média
     
     if(dP <= 2){                                         //Corta os dois primeiros valores (que na maioria das vezes é ruído)   
       dP = 0;
     }else{
       dP = dP-2;
   }
     if(passoDp < numDp){                                 //Esse if coleta os N primeiros dP para popular o array a primeira vez
       dpAnterior[passoDp] = dP;
       passoDp++;
     }else{
       
       sup.avancaTrilha(dpAnterior, dP);                  //Atualiza o array de desvio padrão
       fazSomaDp();                                       //Soma os desvios padrões especificados
       mediaDp = sup.mediaAnteriores(dpAnterior);         //Encontra a média deles
     }
   }
}

int SensorRuido::getVolume(){
 return volumeA;
}

int SensorRuido::getMediaVolume(){
 return media;
}

int SensorRuido::getDesvioPadrao(){
 return dP;
}

int SensorRuido::getSomaDesvioPadrao(){
 return somDp;
}

int SensorRuido::getMediaDesvioPadrao(){
 return mediaDp;
}


void SensorRuido::fazSomaDp(){
  somDp = 0;
 for(int cont = 0; cont < numDp; cont++){
     somDp = somDp + dpAnterior[cont];
 } 
}


