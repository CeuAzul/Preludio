#include <Wire.h>
#include <MS5611.h>

MS5611 ms56112;

double referencePressure2;

void setup()
{
  Wire.begin();
  Serial.begin(9600);

  //Inicia e calibra o módulo MS5611
  setupMS5611();
  calibrateMS5611();
  
  // Aguarda a resposta do sensor
  delay(1500); 
}

void loop()
{

  // Read raw values
  uint32_t rawTemp = ms56112.readRawTemperature();
  uint32_t rawPressure = ms56112.readRawPressure();

  // Read true temperature & Pressure
  double realTemperature = ms56112.readTemperature();
  long realPressure = ms56112.readPressure();

  // Calculate altitude
  float absoluteAltitude = ms56112.getAltitude(realPressure);
  float relativeAltitude = ms56112.getAltitude(realPressure, referencePressure2);
  
  Serial.print(absoluteAltitude);
  Serial.print(",");
  Serial.print(relativeAltitude);
  Serial.print(",");
  Serial.print(realTemperature);
  Serial.print(",");
  Serial.print(realPressure);

}

void setupMS5611()
{
  ms56112.begin();
}

void calibrateMS5611()
{
	// Get reference pressure for relative altitude
	referencePressure2 = ms56112.readPressure();

}


