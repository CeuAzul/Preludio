#include <SensorIMU.h>
#include <SensorPitot.h>
#include <SDHelper.h>
#include <Wire.h>
#include <MenuDeCalibracao.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

SensorIMU imu;
SensorPitot pitot;
SDHelper sdhelp;



void setup(){
  imu.inicializaIMU(5, false, false, true);
  MenuDeCalibracao::iniciaMenuDeCalibracao(11);
  sdhelp.ligaSD();
}

void loop(){
	pitot.atualizaPitot();
	imu.atualizaIMU();


	MenuDeCalibracao::atualizaMenuDados();
	MenuDeCalibracao::setValorUltrassomRaw(10);
	MenuDeCalibracao::setValorPWMRaw(1500, 1600, 1700, 1800);

	String oi[11] = {"Dens ar:"+doubleToString(imu.getDensidadeAr(), 3, 4),
	 "Temp:"+doubleToString(imu.getTemperatura(), 3, 4),
	 "Press:"+doubleToString(imu.getPressao(), 5, 1),
	 "Pitch:"+doubleToString(imu.getPitch(), 3, 4), 
	 "Roll:"+doubleToString(imu.getRoll(), 3, 4),
	 "Solo:"+doubleToString(pitot.getRawPitotSolo(), 4, 4),
	 "Ang:"+doubleToString(pitot.getAnguloAtaquePrimario(), 3, 4),
	 "Front:"+doubleToString(pitot.getRawFrontal(), 3, 4),
	 "Cima:"+doubleToString(pitot.getRawCima(), 3, 4),
	 "Baixo:"+doubleToString(pitot.getRawBaixo(), 3, 4),
	 "Vertical:"+doubleToString(pitot.getRawVertical(), 3, 4)};
	MenuDeCalibracao::setDadosParaExibir(oi);
	sdhelp.abreSD();
	sdhelp.escreveDadoSD(imu.getYaw());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(imu.getPitch());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(imu.getRoll());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(imu.getTemperatura());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(imu.getPressao());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(imu.getDensidadeAr());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawPitotSolo());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawFrontal());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawCima());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawBaixo());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getRawVertical());
	sdhelp.escreveDadoSD(",");
	sdhelp.escreveDadoSD(pitot.getAnguloAtaquePrimario());
	sdhelp.escreveNovaLinhaSD();
	sdhelp.finalizaSD();

}

//Rounds down (via intermediary integer conversion truncation)
String doubleToString(double input,int decimalBefore, int decimalAfter){
    char TempString[12];  //  Hold The Convert Data

    dtostrf(input,decimalBefore,decimalAfter,TempString);
 // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
    return String(TempString);  // cast it to string from char 
}