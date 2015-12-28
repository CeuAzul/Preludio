#include <SDHelper.h>

SDHelper sdhelp;

void setup(){
  	sdhelp.ligaSD();
}

void loop(){
	float a = 123.1234567891011;
	int b = 2;
	double k = 3;
	String oi = "dawd";
	sdhelp.abreSD();
	sdhelp.escreveDadoSD(a, 11);
	sdhelp.escreveNovaLinhaSD();
	sdhelp.escreveDadoSD(b);
	sdhelp.escreveNovaLinhaSD();
	sdhelp.escreveDadoSD(k);
	sdhelp.escreveNovaLinhaSD();
	sdhelp.escreveDadoSD(oi);
	sdhelp.escreveNovaLinhaSD();
	sdhelp.finalizaSD();
	delay(20);
}




