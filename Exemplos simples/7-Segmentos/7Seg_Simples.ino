#include "Display7seg.h"
int tempoparainicio=0;
unsigned long tempoultrassonico;

Display7seg segmentos;
void setup()
{
    Serial.begin(9600);
}
void loop() {
    tempoultrassonico = millis()/100;
    segmentos.mostrarnumero(tempoultrassonico);
}

