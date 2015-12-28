#include <SDHelper.h>

SDHelper sdhelp;

void setup(){
	Serial.begin(9600);
	Serial.println("Digite algo");
	Serial.flush(); //flush all previous received and transmitted data
	while(!Serial.available()) ; // hang program until a byte is received notice the ; after the while()
/*continue program ...*/
	bool deuCerto = sdhelp.leConfigSD();
	Serial.print("Tem Arquivo config? ");
	Serial.println(deuCerto);
	Serial.print("Ultrassom antes: ");
	Serial.println(sdhelp.getUltrassomDist());
	sdhelp.calibraUltrassomNoSD(13);

	deuCerto = sdhelp.leConfigSD();
	Serial.print("E agora? tem Arquivo config? ");
	Serial.println(deuCerto);		
	Serial.print("Ultrassom depois: ");
	Serial.println(sdhelp.getUltrassomDist());
	sdhelp.calibraPWMNoSD(1, 1300, 1863, 65);
	sdhelp.calibraPWMNoSD(2, 1200, 1763, 70);
	sdhelp.calibraPWMNoSD(3, 1400, 1963, 45);
	sdhelp.leConfigSD();
	Serial.print("pwm1Raw0: ");
	Serial.println(sdhelp.getPwm1Raw0());
	Serial.print("getPwm1RawMax: ");
	Serial.println(sdhelp.getPwm1RawMax());
	Serial.print("getPwm1GrauMax: ");
	Serial.println(sdhelp.getPwm1GrauMax());
	Serial.print("pwm2Raw0: ");
	Serial.println(sdhelp.getPwm2Raw0());
	Serial.print("getPwm2RawMax: ");
	Serial.println(sdhelp.getPwm2RawMax());
	Serial.print("getPwm2GrauMax: ");
	Serial.println(sdhelp.getPwm2GrauMax());
	Serial.print("pwm3Raw0: ");
	Serial.println(sdhelp.getPwm2Raw0());
	Serial.print("getPwm3RawMax: ");
	Serial.println(sdhelp.getPwm2RawMax());
	Serial.print("getPwm3GrauMax: ");
	Serial.println(sdhelp.getPwm2GrauMax());
	sdhelp.calibraPWMNoSD(4, 1000, 1500, 85);
	sdhelp.leConfigSD();
	Serial.print("pwm4Raw0: ");
	Serial.println(sdhelp.getPwm4Raw0());
	Serial.print("getPwm4RawMax: ");
	Serial.println(sdhelp.getPwm4RawMax());
	Serial.print("getPwm4GrauMax: ");
	sdhelp.calibraPWMNoSD(1, 90, 5, 7);
	sdhelp.leConfigSD();
	Serial.print("pwm1Raw0: ");
	Serial.println(sdhelp.getPwm1Raw0());
	Serial.print("getPwm1RawMax: ");
	Serial.println(sdhelp.getPwm1RawMax());
	Serial.print("getPwm1GrauMax: ");
	Serial.println(sdhelp.getPwm1GrauMax());
}

void loop(){

}




