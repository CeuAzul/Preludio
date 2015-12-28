// Teste de funções utilizando o sistema operacional

#include <NilRTOS.h>

#include <SDHelper.h>


#include <Csuporte.h>

#include <SensorIMU.h>
#include <Wire.h>
#include "SensorUltrassom.h"

#include <SoftwareSerial.h>
#include <SensorGPS.h>

#include "Display7seg.h"

#include <SensorPwm.h>
#include <SensorPitot.h>
#include <MovingAverageFilter.h>
short ledVerde = A14;
short ledAmarelo = A13;
short ledVermelho = A12;

Display7seg segmentos;
SDHelper sdhelp;
SensorUltrassom ultrassom(15, 3);
SensorGPS gps;
SensorIMU imu;
SensorPitot pitot;
#define DP_CHANNEL_1_PIN 2  
#define CHANNEL_1_INT 0                                     
#define DP_CHANNEL_2_PIN 3  
#define CHANNEL_2_INT 1                                  
#define DP_CHANNEL_3_PIN 18 
#define CHANNEL_3_INT 5                                       
#define DP_CHANNEL_4_PIN 19  
#define CHANNEL_4_INT 4
SensorPwm pwm(DP_CHANNEL_1_PIN, DP_CHANNEL_2_PIN, DP_CHANNEL_3_PIN, DP_CHANNEL_4_PIN);

long tempoIMU;
long tempoGPS;
long tempoPWM;
long tempoUltrassom;
long tempoPitot;
long tempo7seg;
long tempoAcumulado7seg;
long tempoAtualizaLCD;
long tempoDadosLCD;
long tempoSetTelaLCD;
long tempoSD;
long tempoSerial;

//MovingAverageFilter anguloAtaqueF(15);
//MovingAverageFilter anguloDerrapagemF(15);
MovingAverageFilter altitudePressaoF(15);
MovingAverageFilter velocidadeCalibradaF(15);
MovingAverageFilter taxaDeRolamentoF(5);
MovingAverageFilter taxaDeArfagemF(5);
MovingAverageFilter taxaDeGuinadaF(5);
MovingAverageFilter fatorDeCargaXF(5);
MovingAverageFilter fatorDeCargaYF(5);
MovingAverageFilter fatorDeCargaZF(5);
MovingAverageFilter posicaoXF(3);
MovingAverageFilter posicaoYF(3);
MovingAverageFilter posicaoZF(15);
MovingAverageFilter atitudeF(10);
MovingAverageFilter inclinacaoLateralF(10);


NIL_WORKING_AREA(waThread1, 256);				// Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread1, arg){
	while(true){

    long tempoAntes = micros();
    imu.atualizaIMU(); 
    tempoIMU = micros()-tempoAntes;

    nilThdSleep(35);
	}
}

NIL_WORKING_AREA(waThread2, 64);       // Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread2, arg){
  while(true){

    long tempoAntes = micros();
    segmentos.mostraNumero(ultrassom.getTempoDeVoo()/100);
    tempo7seg = micros()- tempoAntes;
    nilThdSleep(20);

  }
}

NIL_WORKING_AREA(waThread3, 64);				// Declara area de trabalho e reserva espaço

NIL_THREAD(Thread3, arg) {
  while (true) {
    long tempoAntes = micros();
    ultrassom.atualizaUltrassom();

    tempoUltrassom = micros() - tempoAntes;
    nilThdSleep(50);
 //   Serial.println("terminou tempo ultra7-seg"); 
  }
}

NIL_WORKING_AREA(waThread4, 64);        // Declara area de trabalho e reserva espaço

NIL_THREAD(Thread4, arg) {
  while (true) {

 //   Serial.println("pwm");
 //   gps.smartdelay(1);
    long tempoAntes = micros();
    pwm.atualizaPWM(); 
    tempoPWM = micros() - tempoAntes;
//    Serial.println("pwm ok");         
    nilThdSleep(19);
 //   Serial.println("terminou tempo pwm"); 

  }
}

NIL_WORKING_AREA(waThread5, 64);        // Declara area de trabalho e reserva espaço

NIL_THREAD(Thread5, arg) {
  while (true) {
    long tempoAntes = micros();
 //   Serial.println("gps");
    gps.smartdelay(1);

    gps.atualizaGPS();
    tempoGPS = micros()-tempoAntes;
 //   Serial.println("gps-ok");
     if(gps.temGPS()){
      digitalWrite(ledVerde, HIGH);
    }else{
      digitalWrite(ledVerde, LOW);
    }
    nilThdSleep(120);   
 //   Serial.println("terminou tempo gps"); 


  }
}



NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread3))
NIL_THREADS_TABLE_ENTRY(NULL, Thread4, NULL, waThread4, sizeof(waThread4))
NIL_THREADS_TABLE_ENTRY(NULL, Thread5, NULL, waThread5, sizeof(waThread5))


NIL_THREADS_TABLE_END()


void setup() {
    pinMode(ledVermelho, OUTPUT);      // sets the digital pin as output
    pinMode(ledAmarelo, OUTPUT);      // sets the digital pin as output
    pinMode(ledVerde, OUTPUT);      // sets the digital pin as output
    digitalWrite(ledVermelho, HIGH);

    attachInterrupt(CHANNEL_1_INT, calcSignalCh1, CHANGE);    //INT-4 AILERON
    attachInterrupt(CHANNEL_2_INT, calcSignalCh2, CHANGE);    //INT-5 Freio
    attachInterrupt(CHANNEL_3_INT, calcSignalCh3, CHANGE);    //INT-3 AILERON
    attachInterrupt(CHANNEL_4_INT, calcSignalCh4, CHANGE);    //INT-2 FREIO

  Serial.begin(9600);
    bool temArquivo = sdhelp.leConfigSD();          // Verifica se tem arquivo de calibração
  if(temArquivo){
    if(sdhelp.getPwm1Raw0() != -9999){
      pwm.calibraPWM(1, sdhelp.getPwm1Raw0(), sdhelp.getPwm1RawMax(), sdhelp.getPwm1GrauMax());
      Serial.println("Calibrou PWM1");    
    }
    if(sdhelp.getPwm2Raw0() != -9999){
      pwm.calibraPWM(2, sdhelp.getPwm2Raw0(), sdhelp.getPwm2RawMax(), sdhelp.getPwm2GrauMax());
      Serial.println("Calibrou PWM2");  
    }
    if(sdhelp.getPwm3Raw0() != -9999){
      pwm.calibraPWM(3, sdhelp.getPwm3Raw0(), sdhelp.getPwm3RawMax(), sdhelp.getPwm3GrauMax());
      Serial.println("Calibrou PWM3");    
    }
    if(sdhelp.getPwm4Raw0() != -9999){
      pwm.calibraPWM(4, sdhelp.getPwm4Raw0(), sdhelp.getPwm4RawMax(), sdhelp.getPwm4GrauMax());
      Serial.println("Calibrou PWM4");    
    }
  }else{
    Serial.println("Não encontrou arquivo de calibracao");
  }

  Serial.println("Calibrando Ultra");
  ultrassom.calibraAquiMesmo(10);


  delay(1000);
  Serial.println("Inicializou sistema"); 

  delay(1000);
   Serial.println("Iniciando Pitot");
  pitot.calibraPitot(10);
  delay(1000);
      Serial.println("Iniciando IMU...");
  imu.inicializaIMU();
    delay(1000);

  Serial.println("Ligando SD...");
  sdhelp.ligaSD();
  setaCabecalho();
    Serial.println("Iniciando RTOS");
  nilSysBegin();

  digitalWrite(ledVermelho, LOW);

}


void loop() {
  long tempoAntesLoop = micros();

    long tempoAntes = micros();
  //  Serial.println("Atualizando pitot");
    pitot.atualizaPitot();
    tempoPitot = micros()-tempoAntes;
    gps.smartdelay(1);


    long tempoAntesSD = micros();
    sdhelp.abreSD();
    if(sdhelp.temArquivoSD()){
      digitalWrite(ledAmarelo, HIGH);
    }else{
      digitalWrite(ledAmarelo, LOW);
    }








    sdhelp.escreveDadoSD(ultrassom.getTempoDeVoo());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getWoWUltrassom());
    sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloAtaqueF.process(adaw));
 //   sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloDerrapagemF.process(awda));
 //   sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(altitudePressaoF.process(imu.getAbsAltitude()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(velocidadeCalibradaF.process(pitot.getRawPitotSolo()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM4());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM1());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM3());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(pwm.getGrauPWM2());
    sdhelp.escreveDadoSD(",");

   sdhelp.escreveDadoSD(taxaDeRolamentoF.process(imu.getXg()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(taxaDeArfagemF.process(imu.getYg()));
    sdhelp.escreveDadoSD(",");
     sdhelp.escreveDadoSD(taxaDeGuinadaF.process(imu.getZg()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(fatorDeCargaXF.process(imu.getXa()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(fatorDeCargaYF.process(imu.getYa()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(fatorDeCargaZF.process(imu.getZa()));
    sdhelp.escreveDadoSD(",");
     sdhelp.escreveDadoSD(posicaoXF.process(gps.getX()), 4);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(posicaoYF.process(gps.getY()), 4);
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(posicaoZF.process(imu.getRelAltitude()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(atitudeF.process(imu.getPitch()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(inclinacaoLateralF.process(imu.getRoll()));
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(millis());

    sdhelp.escreveNovaLinhaSD();
    sdhelp.finalizaSD(); 
    tempoSD = micros()-tempoAntesSD;


  long tempoLoop = micros()-tempoAntesLoop;
  long tempoAntesSerial = micros();
  Serial.print("Tempo IMU: ");
  Serial.print(tempoIMU);
  Serial.print("  Tempo PWM: ");
  Serial.print(tempoPWM);     
  Serial.print("  Tempo GPS: ");
  Serial.print(tempoGPS);
  Serial.print("  Tempo Ultrassom: ");
  Serial.print(tempoUltrassom);     
  Serial.print("  Tempo 7seg: ");
  Serial.print(tempo7seg);
  Serial.print("  Tempo PWM: ");
  Serial.print(tempoPWM);     
  Serial.print("  Tempo pitot: ");
  Serial.print(tempoPitot);   
  Serial.print("  Tempo Atualiza LCD: ");
  Serial.print(tempoAtualizaLCD);
  Serial.print("  Tempo Dados LCD: ");
  Serial.print(tempoDadosLCD);    
  Serial.print("  Tempo setTelaLCD: ");
  Serial.print(tempoSetTelaLCD);  
  Serial.print("  Tempo SD: ");
  Serial.print(tempoSD);  
  Serial.print("  Tempo loop: ");
  Serial.print(tempoLoop);  
  Serial.print("  Tempo Serial: ");
  Serial.println(tempoSerial);  
  tempoSerial = micros()-tempoAntesSerial;

}

void setaCabecalho(){
  Serial.println("Abrindo sd");
        sdhelp.abreSD();
      Serial.println("aberto");
    sdhelp.abreSD();
    sdhelp.escreveDadoSD("Tempo de voo");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Weight on Wheels");
    sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloAtaqueF.process(adaw));
 //   sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloDerrapagemF.process(awda));
//    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Altitude Pressão");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Velocidade Calibrada");
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
    sdhelp.escreveDadoSD("Posição Z");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Atitude");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Inclinação lateral");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Tempo Referência (millis() Arduíno)");

    sdhelp.escreveNovaLinhaSD();

    sdhelp.escreveDadoSD("s");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("Binário");
    sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloAtaqueF.process(adaw));
 //   sdhelp.escreveDadoSD(",");
 //   sdhelp.escreveDadoSD(anguloDerrapagemF.process(awda));
//    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("m");
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
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("°");
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD("ms");
    sdhelp.escreveNovaLinhaSD();

    sdhelp.finalizaSD(); 

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