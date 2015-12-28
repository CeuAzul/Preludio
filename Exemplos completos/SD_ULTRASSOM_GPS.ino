// Teste de funções utilizando o sistema operacional

#include <NilRTOS.h>

#include <SDHelper.h>


#include <Csuporte.h>

#include <SensorIMU.h>
#include <Wire.h>

#include "SensorUltrassom.h"

#include <SoftwareSerial.h>
#include <SensorGPS.h>

#include <MenuDeCalibracao.h>
#include <SDHelper.h>
#include <LiquidCrystal.h>
#include <IRremote.h>


SDHelper sdhelp;
SensorUltrassom ultrassom(15, 3);
SensorGPS gps;
SensorIMU imu;



NIL_WORKING_AREA(waThread1, 64);				// Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread1, arg){
	while(true){
    ultrassom.atualizaUltrassom();

    nilThdSleep(200);
	}
}

NIL_WORKING_AREA(waThread2, 64);				// Declara area de trabalho e reserva espaço

NIL_THREAD(Thread2, arg) {
  while (true) {
    gps.smartdelay(1);
    gps.atualizaGPS();

    nilThdSleep(250);
  }
}



NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()


void setup() {
  MenuDeCalibracao::iniciaMenuDeCalibracao(2);
  Serial.begin(9600);
  sdhelp.ligaSD();
  ultrassom.calibraAquiMesmo(10);
  imu.inicializaIMU(10, false, false, true);

  nilSysBegin();
}


void loop() {
    sdhelp.abreSD();

    MenuDeCalibracao::atualizaMenuDados();
    MenuDeCalibracao::setTela(String(ultrassom.getTempoDeVoo()));

    imu.atualizaIMU(0);

    sdhelp.escreveDadoSD(ultrassom.getTempoDeVoo());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getUltimoTempoEncostadoNoChao());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getContador());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getDistReferencia());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(ultrassom.getDistancia());
    sdhelp.escreveDadoSD(",");

      sdhelp.escreveDadoSD(gps.getNumSat());    
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getPrecisao());     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getLatitude(), 9);        
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getLongitude(), 9); 
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getAge());    
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getAno());    
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getMes());     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getDia());        
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getHora()); 
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getMinuto());   
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getSegundo());     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getCentSegundo());        
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getAltitude(), 4);   
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getCurse(), 4);     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getVelocidade(), 4);        
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getX(), 4);   
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getY(), 4);     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getDist(), 4);        
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getLatRef(), 9);     
      sdhelp.escreveDadoSD(",");
      sdhelp.escreveDadoSD(gps.getLongRef(), 9); 
      sdhelp.escreveDadoSD(","); 

    sdhelp.escreveDadoSD(imu.getYaw());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getPitch());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getRoll());
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
    sdhelp.escreveDadoSD(imu.getTemperatura());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getPressao());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getAbsAltitude());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getDensidadeAr());
    sdhelp.escreveDadoSD(",");
    sdhelp.escreveDadoSD(imu.getRelAltitude());
    sdhelp.escreveDadoSD(",");


    sdhelp.escreveNovaLinhaSD();

    sdhelp.finalizaSD(); 
    Serial.println("ada");   

}