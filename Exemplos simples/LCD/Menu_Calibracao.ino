#include <MenuDeCalibracao.h>
#include <SDHelper.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

void setup(){
  MenuDeCalibracao::iniciaMenuDeCalibracao(2);
  Serial.begin(9600);
}




void loop(){
	MenuDeCalibracao::atualizaMenuDados();
	MenuDeCalibracao::setValorUltrassomRaw(10);
	MenuDeCalibracao::setValorPWMRaw(1500, 1600, 1700, 1800);
	String oi[2] = {"oi:150", "Teste"};
	MenuDeCalibracao::setDadosParaExibir(oi);
	delay(150);

}