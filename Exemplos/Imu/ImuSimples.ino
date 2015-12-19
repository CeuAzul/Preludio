#include <SensorIMU.h>
#include <Wire.h>
SensorIMU imu;
long tempoImu;

void setup(){

  Serial.begin(9600);            // Inicia a comunicação Serial com taxa de 9600
  delay(1000);                   // Aguarda a resposta do sensor
  imu.inicializaIMU(10, false, false, true);
   Serial.println("Digite algo");
  Serial.flush();        //flush all previous received and transmitted data
  while(!Serial.available()) ; // hang program until a byte is received notice the ; after the while()
  /*continue program ...*/
}

void loop(){
	tempoImu = micros();
	imu.atualizaIMU(0);

	tempoImu = micros() - tempoImu;
	Serial.print(tempoImu);
	Serial.print(",  ");
	Serial.print(imu.getYaw());
	Serial.print(",  ");
	Serial.print(imu.getPitch());
	Serial.print(",  ");
	Serial.print(imu.getRoll());
	Serial.print(",  ");
	Serial.print(imu.getXg());
	Serial.print(",  ");
	Serial.print(imu.getYg());
	Serial.print(",  ");
	Serial.print(imu.getZg());
	Serial.print(",  ");
	Serial.print(imu.getXa());
	Serial.print(",  ");
	Serial.print(imu.getYa());
	Serial.print(",  ");
	Serial.print(imu.getZa());
	Serial.print(",  ");
	Serial.print(imu.getTemperatura());
	Serial.print(",  ");
	Serial.print(imu.getPressao());
	Serial.print(",  ");
	Serial.print(imu.getDensidadeAr());
	Serial.print(",  ");
	Serial.print(imu.getAbsAltitude());
	Serial.print(",  ");
	Serial.print(imu.getRelAltitude());
	Serial.println(",  ");
}
