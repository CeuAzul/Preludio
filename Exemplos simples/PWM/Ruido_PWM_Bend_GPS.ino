#include <Csuporte.h>          // Biblioteca com fuções de suporte (filtragem, map etc...)
#include <SoftwareSerial.h>    // Biblioteca Serial para o GPS
#include <SensorGPS.h>         // Biblioteca com funções do GPS
#include <TinyGPS.h>           // Biblioteca de base para comunicação com GPS

#include <NewPingCA.h>         // Bilblioteca de base pra o ultrassom
#include "SensorUltrassom.h"   // Bibliotecas de funções do ultrassom

#include <SensorRuido.h>       // Biblioteca de funções do sensor de ruído

#include <SensorPwm.h>         // Biblioteca com funções do PWM, bend e RPM

#include <SDHelper.h>          // Biblioteca puta sensacional pra escrita no SD
#include <SD.h>                // Bilbioteca nativa, base para a escrita no SD

int PA_SENSOR_DE_RUIDO = A0;


#define TRIGGER_PIN  5          // Trigger pin do Ultrassom.
#define ECHO_PIN     4          // Echo pin do ultrassom.
#define MAX_DISTANCE 450        // Distância máxima de detecção do ultrassom (em centímetros) 400-500cm.

#define AP_BENDSENSPIN A1       // Pino do Bend sensor
#define AP_VRPMPIN A9           // Pino da tensão do RPM

#define DP_CHANNEL_1_PIN 2      // Número do pino PWM 1
#define CHANNEL_1_INT 0         // Número da interrupção
                                                           
                                           
#define DP_CHANNEL_2_PIN 3      // Número do pino PWM 2
#define CHANNEL_2_INT 1         // Número da interrupção
                                       
#define DP_CHANNEL_3_PIN 18     // Número do pino PWM 3
#define CHANNEL_3_INT 5         // Número da interrupção

                                          
#define DP_CHANNEL_4_PIN 19     // Número do pino PWM 4 (Leitor do PWM)
#define CHANNEL_4_INT 4         // Número da interrupção


SensorGPS gps;
SensorUltrassom ultrassom(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
SensorRuido sensorRuido(PA_SENSOR_DE_RUIDO);
SensorPwm pwm(DP_CHANNEL_1_PIN, DP_CHANNEL_2_PIN, DP_CHANNEL_3_PIN, DP_CHANNEL_4_PIN, AP_BENDSENSPIN);
SDHelper sdhelp;


void setup() {
  attachInterrupt(CHANNEL_1_INT, calcSignalCh1, CHANGE);    //INT-4 RPM
  attachInterrupt(CHANNEL_2_INT, calcSignalCh2, CHANGE);    //INT-5 Freio
  attachInterrupt(CHANNEL_3_INT, calcSignalCh3, CHANGE);    //INT-3 AILERON
  attachInterrupt(CHANNEL_4_INT, calcSignalCh4, CHANGE);    //INT-2 FREIO
  Serial.begin(9600);

  delay(1000);                   // Aguarda a resposta do sensor
  sdhelp.ligaSD();

}

void loop() {
  gps.atualizaGPS();
  ultrassom.atualizaUltrassom();
  pwm.atualizaPWM();
  sensorRuido.atualizaRuido();

  sdhelp.abreSD();

   sdhelp.escreveDadoSD(gps.getNumSat());    
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getPrecisao());     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getLatitude(), 11);        
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getLongitude(), 11); 
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getAge());    
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getAno());    
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getMes());     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getDia());        
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getHora()); 
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getMinuto());   
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getSegundo());     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getCentSegundo());        
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getAltitude(), 4);   
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getCurse(), 4);     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getVelocidade(), 4);        
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getX(), 4);   
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getY(), 4);     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getDist(), 4);        
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getLatRef(), 11);     
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(gps.getLongRef(), 11); 
   sdhelp.escreveDadoSD(",  "); 

   sdhelp.escreveDadoSD(ultrassom.getTempoDeVoo());
   sdhelp.escreveDadoSD(",  "); 
   sdhelp.escreveDadoSD(ultrassom.getUltimoTempoEncostadoNoChao());
   sdhelp.escreveDadoSD(",  "); 
   sdhelp.escreveDadoSD(ultrassom.getDistancia());

   sdhelp.escreveDadoSD(",  "); 
   sdhelp.escreveDadoSD(sensorRuido.getVolume());
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(sensorRuido.getMediaVolume());
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(sensorRuido.getDesvioPadrao());
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(sensorRuido.getSomaDesvioPadrao());
   sdhelp.escreveDadoSD(",  ");
   sdhelp.escreveDadoSD(sensorRuido.getMediaDesvioPadrao());
   sdhelp.escreveDadoSD(",  ");


  sdhelp.escreveDadoSD(pwm.getGrauPWM1());
  sdhelp.escreveDadoSD(",   ");
  sdhelp.escreveDadoSD(pwm.getGrauPWM2());
  sdhelp.escreveDadoSD(",   ");
  sdhelp.escreveDadoSD(pwm.getGrauPWM3());
  sdhelp.escreveDadoSD(",   ");
  sdhelp.escreveDadoSD(pwm.getGrauPWM4());
  sdhelp.escreveDadoSD(",   ");
  sdhelp.escreveDadoSD(pwm.getGrauBend());
  sdhelp.escreveDadoSD(",   ");
  sdhelp.escreveDadoSD(pwm.getGrauBendMedia());
  sdhelp.escreveDadoSD(",   ");

  sdhelp.escreveNovaLinhaSD();
  sdhelp.finalizaSD();

  gps.smartdelay(0);
}

void calcSignalCh1(){                     
  pwm.sinalCH1();
}                          

void calcSignalCh2(){                       
  pwm.sinalCH2();
}

void calcSignalCh3(){                 
  pwm.sinalCH3();
}

void calcSignalCh4(){                   
  pwm.sinalCH4();
}