#include <SensorChave.h>

SensorChave chaveDoTempo(300);

void setup() {
    Serial.begin(9600);                                      //inicia a porta serial
}

void loop() {
  chaveDoTempo.atualizaChave();

  Serial.print(chaveDoTempo.getContadorChave());
  Serial.print(",   "); 
  Serial.print(chaveDoTempo.getTempoDeVooChave());
  Serial.print(",   "); 
  Serial.print(chaveDoTempo.getContadorChave());
  Serial.print(",   "); 
  Serial.print(chaveDoTempo.getUltimoTempoEncostadoNoChaoChave());
  Serial.print(",   "); 
  Serial.print(chaveDoTempo.getTempoDeConfirmacao());
  Serial.print(",   "); 
  Serial.println(chaveDoTempo.getWOW());

  delay(300);
}
