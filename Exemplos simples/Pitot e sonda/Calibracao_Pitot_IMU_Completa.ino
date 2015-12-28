#include <SensorIMU.h>
#include <SensorPitot.h>
#include <SDHelper.h>
#include <Wire.h>

#include <MenuDeCalibracao.h>
#include <SDHelper.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

SensorIMU imu;
SensorPitot pitot;
SDHelper sdhelp;



void setup(){
  Serial.begin(9600);
  imu.inicializaIMU(10, false, false, true);
  MenuDeCalibracao::iniciaMenuDeCalibracao(2);
  sdhelp.ligaSD();
  pitot.calibraPitot(10);
}

void loop(){
	pitot.atualizaPitot();
	imu.atualizaIMU();
	int velcap = pitot.getVelocidadeCalibrada();

	MenuDeCalibracao::setTela(String(velcap));


	sdhelp.abreSD();
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
	sdhelp.escreveDadoSD(pitot.getRawPitotSolo());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawFrontal());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawCima());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawBaixo());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawEsquerda());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawDireita());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawVertical());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawHorizontal());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getVelocidadeCalibrada());
	sdhelp.escreveNovaLinhaSD();
	sdhelp.finalizaSD();

	Serial.print(pitot.getRawFrontal());
	Serial.print(";");
	Serial.print(pitot.getRawCima());
	Serial.print(";");
	Serial.print(pitot.getRawBaixo());      // frente cima?
	Serial.print(";");
	Serial.print(pitot.getRawEsquerda());  // cima
	Serial.print(";");
	Serial.println(pitot.getRawDireita());// frente
//	Serial.print(";");
//	Serial.print(pitot.getRawPitotSolo());  // cima
//	Serial.print(";");
//	Serial.println(pitot.getVelocidadeCalibrada());// frente

	delay(20);


}

//Rounds down (via intermediary integer conversion truncation)
String doubleToString(double input,int decimalBefore, int decimalAfter){
    char TempString[12];  //  Hold The Convert Data

    dtostrf(input,decimalBefore,decimalAfter,TempString);
 // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
    return String(TempString);  // cast it to string from char 
}