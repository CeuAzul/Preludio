#include "SensorUltrassom.h"

SensorUltrassom ultrassom(15, 3);

void setup() {
    Serial.begin(9600);                                      //inicia a porta serial
    ultrassom.calibraAquiMesmo(10);
}

void loop() {
  ultrassom.atualizaUltrassom();
  Serial.print(ultrassom.getTempoDeVoo());
  Serial.print(",   "); 
  Serial.print(ultrassom.getContador());
  Serial.print(",   "); 
  Serial.print(ultrassom.getUltimoTempoEncostadoNoChao());
  Serial.print(",   "); 
  Serial.print(ultrassom.getDistReferencia());
  Serial.print(",   "); 
  Serial.println(ultrassom.getDistancia());

  delay(300);
}
