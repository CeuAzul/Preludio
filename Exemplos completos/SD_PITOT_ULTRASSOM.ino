#include <NilRTOS.h>

#include <SDHelper.h>

#include <Csuporte.h>

#include <Wire.h>
#include "SensorUltrassom.h"

#include <LiquidCrystal.h>
#include <SensorPitot.h>

#include <MovingAverageFilter.h>

MovingAverageFilter velocidadeCalibradaF(100);

short ledVerde = A14;
short ledAmarelo = A13;
short ledVermelho = A12;

SensorPitot pitot;
SDHelper sdhelp;
SensorUltrassom ultrassom(15, 3);

float tempoVoo;

LiquidCrystal lcd(22, 24, 26, 28, 30, 32, 34);

int tempoAntesVirgula;
int tempoDepoisVirgula;
int tempoTotal;
float tempoRef;

NIL_WORKING_AREA(waThread1, 256);       // Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread1, arg){
  while(true){
    nilThdSleep(50);
    if(sdhelp.temArquivoSD()){
      digitalWrite(ledAmarelo, HIGH);
    }else{
      digitalWrite(ledAmarelo, LOW);
    }
  }
}


NIL_WORKING_AREA(waThread2, 128);        

NIL_THREAD(Thread2, arg) {
  while (true) {
    ultrassom.atualizaUltrassom();
    nilThdSleep(50);
  }
}


NIL_WORKING_AREA(waThread3, 128);       // Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread3, arg){
  while(true){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Tempo voo = ");
        lcd.setCursor(0,1);
        tempoTotal =ultrassom.getTempoDeVoo();
        tempoAntesVirgula = ultrassom.getTempoDeVoo()/1000;
        tempoDepoisVirgula = tempoTotal - tempoAntesVirgula*1000;
        lcd.print(tempoAntesVirgula);
        lcd.print(",");
        lcd.print(tempoDepoisVirgula);
        lcd.print(" s");
        nilThdSleep(1000);
  }
}






NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread3))

NIL_THREADS_TABLE_END()


void setup() {
  Serial.begin(9600);
    pinMode(ledVermelho, OUTPUT);      // sets the digital pin as output
    pinMode(ledAmarelo, OUTPUT);      // sets the digital pin as output
    pinMode(ledVerde, OUTPUT);      // sets the digital pin as output
    digitalWrite(ledVermelho, HIGH);

  ultrassom.calibraAquiMesmo(10);
  delay(1000);
  pitot.calibraPitot(40);
  delay(1000);
  lcd.begin(16, 2);
  delay(1000);
  sdhelp.ligaSD();
  setaCabecalho();
  nilSysBegin();
  digitalWrite(ledVermelho, LOW);

}


void loop() {
    pitot.atualizaPitot();

    sdhelp.abreSD();
    tempoRef = ((float)(millis()))/ ((float)1000);
    tempoVoo = ((float)ultrassom.getTempoDeVoo())/ ((float)1000);



    sdhelp.escreveDadoSD(tempoRef, 2);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getWoWUltrassom());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getWoWRaw());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pitot.getRawPitotSolo());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pitot.getVelocidadeCalibrada());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(velocidadeCalibradaF.process(pitot.getVelocidadeCalibrada()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(tempoVoo, 2);

    sdhelp.escreveNovaLinhaSD();
    sdhelp.finalizaSD(); 

}

void setaCabecalho(){
    sdhelp.abreSD();
    sdhelp.escreveDadoSD("Tempo Referência");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Wow Filtrado");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Wow raw");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Pitot velocidade raw");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Velocidade calibrada");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Velocidade calibrada filtrada (100)");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Tempo de voo");


    sdhelp.escreveNovaLinhaSD();

    sdhelp.escreveDadoSD("sec");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("bin");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("bin");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Analog Volt");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("sec");

    sdhelp.escreveNovaLinhaSD();

    sdhelp.finalizaSD(); 
    digitalWrite(ledVerde, HIGH);


}
