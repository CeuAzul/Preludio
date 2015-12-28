#include <Csuporte.h>
#include <SensorRuido.h>


int PA_SENSOR_DE_RUIDO = A5;

SensorRuido sensorRuido(PA_SENSOR_DE_RUIDO);

void setup() {                
   Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  sensorRuido.atualizaRuido();
  Serial.print(sensorRuido.getVolume());
  Serial.print(",");
  Serial.print(sensorRuido.getMediaVolume());
  Serial.print(",");
  Serial.print(sensorRuido.getDesvioPadrao());
  Serial.print(",");
  Serial.print(sensorRuido.getSomaDesvioPadrao());
  Serial.print(",");
  Serial.print(sensorRuido.getMediaDesvioPadrao());
  Serial.println("............");

  delay(50);
}
