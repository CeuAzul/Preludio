#include "Arduino.h"
#include "SensorPitot.h"

int pitotSamples = 50;

double rawFrontal;
double rawCima;
double rawBaixo;
double rawEsquerda;
double rawDireita;
double rawPitotSolo;
double rawVertical;
double rawHorizontal;

double anguloAtaquePrimario;
double pressaoVelocidade;
double velocidadeCalibrada;

double rawZeroPitotSolo;

int pitotSolo;
int pressaoDinamica;
int ataqueUP;
int ataqueDOWN;
int bankRIGHT;
int bankLEFT;



SensorPitot::SensorPitot(int nSamples, int pinPitotSolo, int pinPessaoDinamica, int pinAtaqueUP, int pinAtaqueDOWN, int pinBankRIGHT, int pinBankLEFT) {
  pressaoDinamica = pinPessaoDinamica;
  ataqueUP = pinAtaqueUP;
  ataqueDOWN = pinAtaqueDOWN;
  bankRIGHT = pinBankRIGHT;
  bankLEFT = pinBankLEFT;
  pitotSamples = nSamples;
  pitotSolo = pinPitotSolo;
}

void SensorPitot::calibraPitot(int nSamples) {
  for(int i=0; i<nSamples; i++) {
    rawZeroPitotSolo += analogRead(pitotSolo);
    delay(5);
  }
  rawZeroPitotSolo = (rawZeroPitotSolo/nSamples);
    Serial.print("raw solo: ");  // cima
    Serial.print(rawZeroPitotSolo);  // cima
    Serial.println(";");
}


void SensorPitot::atualizaPitot(double densAr) {
  rawFrontal = 0;
  rawCima = 0;
  rawBaixo = 0;
  rawEsquerda = 0;
  rawDireita = 0;
  rawPitotSolo = 0;

  for(int i=0; i<pitotSamples; i++) {
   // rawFrontal += analogRead(pressaoDinamica);
   // rawCima += analogRead(ataqueUP);
   // rawBaixo += analogRead(ataqueDOWN);
   // rawDireita += analogRead(bankRIGHT);
   // rawEsquerda += analogRead(bankLEFT);
    rawPitotSolo += analogRead(pitotSolo);
        delay(5);
  }

  rawFrontal = (rawFrontal/pitotSamples);
 // rawFrontal = rawFrontal-160;
  rawCima = (rawCima/pitotSamples);
 // rawCima = rawCima - 158;
  rawBaixo = (rawBaixo/pitotSamples);
  rawEsquerda = (rawEsquerda/pitotSamples);
  rawDireita = (rawDireita/pitotSamples);
 // rawBaixo = rawBaixo - 150;
  rawPitotSolo = (rawPitotSolo/pitotSamples);


  rawVertical = rawCima - rawBaixo;
  rawHorizontal = rawDireita - rawEsquerda;

  pressaoVelocidade = (rawPitotSolo-rawZeroPitotSolo)/0.0974;
  velocidadeCalibrada = sqrt(abs((pressaoVelocidade*2)/densAr));
  if(pressaoVelocidade < 0){
    velocidadeCalibrada = velocidadeCalibrada*(-1);
  }


}

double SensorPitot::getRawFrontal(){
  return rawFrontal;
}

double SensorPitot::getRawCima(){
  return rawCima;
}

double SensorPitot::getRawBaixo(){
  return rawBaixo;
}

double SensorPitot::getRawEsquerda(){
  return rawEsquerda;
}

double SensorPitot::getRawDireita(){
  return rawDireita;
}

double SensorPitot::getRawVertical(){
  return rawVertical;
}

double SensorPitot::getRawHorizontal(){
  return rawHorizontal;
}

double SensorPitot::getRawPitotSolo(){
  return rawPitotSolo;
}

double SensorPitot::getPressaoPitot(){
  return pressaoVelocidade;
}

double SensorPitot::getVelocidadeCalibrada(){
  return velocidadeCalibrada;
}
