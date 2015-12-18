
/*
  SensorBend- Biblioteca de funções relacionadas ao bend sensor
*/

#include "Arduino.h"
#include "SensorBend.h"
#include <Csuporte.h>

namespace variaveisBend{
  int analogPin;
  const int numPros = 20;                        //Número de valores anteriores a serem analizados
  float dataAnterior[numPros];                  //Array onde os valores anteriores estão estocados
  int passo = 0;                                //Variável vai cuidar dos N primeiros valores
  float media = 0;                              //Média dos N ultimos valores
  float sensor, grauBend;
  Csuporte sup;
}

using namespace variaveisBend;

int BEND_PINO;

const int GRAUMAX = 0;
const int GRAUMIN = 1;
const int ANALOG0 = 2;
const int ANALOGMAX = 3;
const int ANALOGMIN = 4;

bool processa = true;
float calibrBEND[5];




//this is all normal arduino stuff
SensorBend::SensorBend(int canalBend){
    pinMode(canalBend, INPUT);           // Input para o bend


    calibrBEND[GRAUMAX] = 10.5;          // Calibração do bend sensor
    calibrBEND[GRAUMIN] = -16;      
    calibrBEND[ANALOG0] = 471.6;         // Colocar aqui o valor que vem direto do sensor, quando o ângulo for 0 graus
    calibrBEND[ANALOGMAX] = 527.00;       // Colocar aqui o valor que vem direto do sensor quando o angulo for GRAUMAX
    calibrBEND[ANALOGMIN] = 506.00;         // Colocar aqui o valor que vem direto do sensor quando o angulo for GRAUMIN

    BEND_PINO = canalBend;

}

void SensorBend::atualizaBend(){
  sensor = analogRead(BEND_PINO);             // Le a tensão do divisor de tensão (sensor plus resistor)
  if(processa){                               // Se tiver que processar o dado
    if(passo < numPros){                      // Esse if coleta os N primeiros valores (para encher o array a primeira vez)
       dataAnterior[passo] = sensor;
       passo++;
       
    }else{ 
        sup.avancaTrilha(dataAnterior, sensor);                       // Último valor vira o penúltimo, o penultimo vira o ante-penúltimo, e por aí vai
        media = sup.mediaAnteriores(dataAnterior);                    // Atualiza o int 'media' com o novo valor de média
    }

    if(media >= calibrBEND[ANALOG0]){
       grauBend = sup.mapfloat(media, calibrBEND[ANALOG0], calibrBEND[ANALOGMAX], 0, calibrBEND[GRAUMAX]);       //Converte a tensão do Bend Sensor para angulo
    }else{
       grauBend =  sup.mapfloat(media, calibrBEND[ANALOG0], calibrBEND[ANALOGMIN], 0, calibrBEND[GRAUMIN]);
    }
  }
}

// Modo de operação --------------------------------------------------------------------
// Essa função vai dizer pro programa se precisa ou não processar os dados 
// (Se quiser apenas a saída analógica pura, setar como falso)

void setProcessarDados(bool devoProcessar){
    processa = devoProcessar;
}





// Funções de calibração ----------------------------------------------------------------
// Essas funções fazem a linkagem do valor puro da entrada analógica com o valor da angulação fornecido pelo usuário

void SensorBend::calibraComoMaximo(float grauMaximo){
  calibrBEND[GRAUMAX] = grauMaximo;                        // Seta grau máximo de referencia com o fornecido pelo usuário
  calibrBEND[ANALOGMAX] = analogRead(BEND_PINO);           // Pega valor do analógico e seta isso como a referencia

}
void SensorBend::calibraComoZero(){
  calibrBEND[ANALOG0] = analogRead(BEND_PINO);             // O valor desse analógico será o nosso angulo 0

}
void SensorBend::calibraComoMinimo(float grauMinimo){
  calibrBEND[GRAUMIN] = grauMinimo;                        // Seta grau minimo de referencia com o fornecido pelo usuário
  calibrBEND[ANALOGMIN] = analogRead(BEND_PINO);           // Pega valor do analógico e seta isso como a referencia do grau mínimo


}
// Getters-----------------------------------------------------------------------------

float SensorBend::getGrauBend(){
  return grauBend;
}

float SensorBend::getGrauBendMedia(){
  return media;
}

float SensorBend::getGrauBendRaw(){
  return sensor;
}

float SensorBend::getCalibrAnalogMax(){
  return calibrBEND[ANALOGMAX];
}

float SensorBend::getCalibrAnalogMin(){
  return calibrBEND[ANALOGMIN];
}

float SensorBend::getCalibrAnalogZero(){
  return calibrBEND[ANALOG0];
}

