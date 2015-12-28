// Teste de funções utilizando o sistema operacional

#include <NilRTOS.h>

#include <SDHelper.h>

#include <Csuporte.h>

#include <SensorIMU.h>
#include <Wire.h>
#include "SensorUltrassom.h"

#include <SensorPitot.h>

#include <SoftwareSerial.h>
#include <SensorGPS.h>

#include <LiquidCrystal.h>
#include <SensorPwm.h>
#include <SensorPitot.h>
#include <MovingAverageFilter.h>
short ledVerde = A14;
short ledAmarelo = A13;
short ledVermelho = A12;

SDHelper sdhelp;
SensorUltrassom ultrassom(15, 3);
SensorGPS gps;
SensorIMU imu;
SensorPitot pitot;
float tempoVoo;
#define DP_CHANNEL_1_PIN 2  
#define CHANNEL_1_INT 0                                     
#define DP_CHANNEL_2_PIN 3  
#define CHANNEL_2_INT 1                                  
#define DP_CHANNEL_3_PIN 18 
#define CHANNEL_3_INT 5                                       
#define DP_CHANNEL_4_PIN 19  
#define CHANNEL_4_INT 4
SensorPwm pwm(DP_CHANNEL_1_PIN, DP_CHANNEL_2_PIN, DP_CHANNEL_3_PIN, DP_CHANNEL_4_PIN);
LiquidCrystal lcd(22, 24, 26, 28, 30, 32, 34);

int tempoAntesVirgula;
int tempoDepoisVirgula;
int tempoTotal;
float tempoRef;
int tempoInicioSD = -1;


MovingAverageFilter velocidadeCalibradaF(40);
MovingAverageFilter posicaoZF(10);

long tempoAntesPWM = millis();
int vez=0;
int ch11, ch12, ch13, ch14;

NIL_WORKING_AREA(waThread1, 256);       // Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread1, arg){
  while(true){
  if((millis()-tempoAntesPWM)>100){
  //  noInterrupts();
    switch (vez) {
        case 0:
          ch11 = pulseIn(DP_CHANNEL_1_PIN, HIGH, 20000); 
          vez++;
          break;
        case 1:
          ch12 = pulseIn(DP_CHANNEL_2_PIN, HIGH, 20000);
          vez++; 
          break;
        case 2:
          ch13 = pulseIn(DP_CHANNEL_3_PIN, HIGH, 20000);
          vez++; 
          break;
        case 3:
          ch14 = pulseIn(DP_CHANNEL_4_PIN, HIGH, 20000);
          vez = 0;
          break;
        default:
          vez = 0;
    }
   // interrupts();
    pwm.pegaAngulo1(ch11);
    pwm.pegaAngulo2(ch12);
    pwm.pegaAngulo3(ch13);
    pwm.pegaAngulo4(ch14);
  }
    imu.atualizaIMU(); 
    nilThdSleep(38);
          if(sdhelp.temArquivoSD()){
      digitalWrite(ledAmarelo, HIGH);
    }else{
      digitalWrite(ledAmarelo, LOW);
    }
  }
}

NIL_WORKING_AREA(waThread2, 128);       // Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread2, arg){
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

NIL_WORKING_AREA(waThread3, 128);        

NIL_THREAD(Thread3, arg) {
  while (true) {
    ultrassom.atualizaUltrassom();
    nilThdSleep(50);
  }
}

NIL_WORKING_AREA(waThread4, 128);       

NIL_THREAD(Thread4, arg) {
  while (true) {
    gps.smartdelay(1);
    gps.atualizaGPS();
    if(gps.temGPS()){
      digitalWrite(ledVerde, HIGH);
    }else{
      digitalWrite(ledVerde, LOW);
    }
    nilThdSleep(80);   


  }
}



NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread3))
NIL_THREADS_TABLE_ENTRY(NULL, Thread4, NULL, waThread4, sizeof(waThread4))


NIL_THREADS_TABLE_END()


void setup() {
    pinMode(ledVermelho, OUTPUT);      // sets the digital pin as output
    pinMode(ledAmarelo, OUTPUT);      // sets the digital pin as output
    pinMode(ledVerde, OUTPUT);      // sets the digital pin as output
    digitalWrite(ledVermelho, HIGH);

    bool temArquivo = sdhelp.leConfigSD();          // Verifica se tem arquivo de calibração
  if(temArquivo){
    if(sdhelp.getPwm1Raw0() != -9999){
      pwm.calibraPWM(1, sdhelp.getPwm1Raw0(), sdhelp.getPwm1RawMax(), sdhelp.getPwm1GrauMax()); 
    }
    if(sdhelp.getPwm2Raw0() != -9999){
      pwm.calibraPWM(2, sdhelp.getPwm2Raw0(), sdhelp.getPwm2RawMax(), sdhelp.getPwm2GrauMax()); 
    }
    if(sdhelp.getPwm3Raw0() != -9999){
      pwm.calibraPWM(3, sdhelp.getPwm3Raw0(), sdhelp.getPwm3RawMax(), sdhelp.getPwm3GrauMax()); 
    }
    if(sdhelp.getPwm4Raw0() != -9999){
      pwm.calibraPWM(4, sdhelp.getPwm4Raw0(), sdhelp.getPwm4RawMax(), sdhelp.getPwm4GrauMax());  
    }
  }else{
  }
  ultrassom.calibraAquiMesmo(10);
  delay(1000);
  pitot.calibraPitot(100);
  delay(1000);
  imu.inicializaIMU();
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

    gps.smartdelay(1);

    sdhelp.abreSD();

 //   nilThdSleep(80);

    if(tempoInicioSD == -1){
      tempoInicioSD = millis();
    }
    tempoRef = ((float)(millis()-tempoInicioSD))/ ((float)1000);
    tempoVoo = ((float)ultrassom.getTempoDeVoo())/ ((float)1000);



    sdhelp.escreveDadoSD(tempoRef, 2);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getWoWUltrassom());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getAbsAltitude());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pitot.getRawPitotSolo());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pitot.getVelocidadeCalibrada());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(velocidadeCalibradaF.process(pitot.getVelocidadeCalibrada()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(gps.getVelocidade());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM4());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM1());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM3());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM2());
    sdhelp.escreveDadoSD(",");

   sdhelp.escreveDadoSD(imu.getXg());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getYg());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getZg());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getXa());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getYa());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getZa());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(gps.getX(), 4);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(gps.getY(), 4);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(posicaoZF.process(imu.getRelAltitude()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getRelAltitude());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getPitch());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getRoll());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(tempoVoo, 2);

    sdhelp.escreveNovaLinhaSD();
    sdhelp.finalizaSD(); 

}

void setaCabecalho(){
    sdhelp.abreSD();
    sdhelp.escreveDadoSD("Tempo");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Weight on Wheels");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Altitude Pressão");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Pitot raw");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Pitot velocidade calibrada");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Pitot velocidade calibrada filtrado");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Velocidade Calibrada GPS");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Rotação da hélice");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Deflexão profundor");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Deflexão aileron");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Deflexão leme");
    sdhelp.escreveDadoSD(",");

    sdhelp.escreveDadoSD("Taxa de rolamento");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Taxa de arfagem");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Taxa de guinada");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Fator de carga X");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Fator de carga Y");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Fator de carga Z");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Posição X");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Posição Y");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Posição Z filtrado");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Posição Z");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Atitude");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Inclinação lateral");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Tempo de voo");


    sdhelp.escreveNovaLinhaSD();

    sdhelp.escreveDadoSD("s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("-");
    sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloAtaqueF.process(adaw));
 //   sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloDerrapagemF.process(awda));
//    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("volt analog");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("rpm");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");

    sdhelp.escreveDadoSD("°/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°/s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("g");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("g");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("g");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("s");
    sdhelp.escreveNovaLinhaSD();

    sdhelp.finalizaSD(); 

}
