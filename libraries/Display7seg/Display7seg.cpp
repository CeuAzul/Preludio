  #include "Arduino.h"
  #include "Display7seg.h"
  #define segmento_ligado  LOW
  #define segmento_desligado HIGH
  #define DIGIT_ON  HIGH
  #define DIGIT_OFF  LOW
  #define DISPLAY_BRIGHTNESS  4000   //LUMINOSIDADE VALOR DA CORRENTE POR DIGITO:
                                  //5000 (15.7mA)
                                  //2000 (11.4mA)
                                  //1000 (5.9mA)
                                  //500  (3mA)
                                  //200  (1.4mA)
                                  //50   (0.56mA)
                                  //5    (0.31mA)
                                  //1 APENAS VISIVEL NO ESCURO(0.28mA)
  int digit1; //displa 12
  int digit2;//Anodo 9
  int digit3;// Anodo 8
  int digit4; //Anodo 6
  int segA; // display 11
  int segB; // dysplay 7
  int segC; // display 4
  int segD; // display 2
  int segE; // display 1
  int segF; // display 10
  int segG; // display 5
  int ponto; // ponto
 
  
Display7seg::Display7seg(int digit11, int segA1, int segF1, int digit21, int digit31, int segB1, int segE1, int segD1, int pto, int segC1, int segG1, int digit41) {
  segA=segA1;
  segB=segB1;
  segC=segC1;
  segD=segD1;
  segE=segE1;
  segF=segF1;
  segG=segG1;
  digit1=digit11;
  digit2=digit21;
  digit3=digit31;
  digit4=digit41;
  ponto = pto;
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  pinMode(ponto, OUTPUT);

  }
  
void Display7seg::mostraNumero(int toDisplay) {
int beginTime=millis();
for(int digit = 4 ; digit > 0 ; digit--) {
     switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      digitalWrite(ponto, segmento_desligado);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      digitalWrite(ponto, segmento_desligado);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      digitalWrite(ponto, segmento_ligado);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      digitalWrite(ponto, segmento_desligado);
      break;
    }

    //Turn on the right segments for this digit
    iluminarnumero(toDisplay % 10);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    iluminarnumero(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  while( (millis() - beginTime) < 10) ; 
  //Wait for 20ms to pass before we paint the display again
}

void Display7seg::iluminarnumero(int numberToDisplay) {
  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_ligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_desligado);
    break;

  case 1:
    digitalWrite(segA, segmento_desligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_desligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_desligado);
    digitalWrite(segG, segmento_desligado);
    break;

  case 2:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_desligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_ligado);
    digitalWrite(segF, segmento_desligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 3:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_desligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 4:
    digitalWrite(segA, segmento_desligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_desligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 5:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_desligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 6:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_desligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_ligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 7:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_desligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_desligado);
    digitalWrite(segG, segmento_desligado);
    break;

  case 8:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_ligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 9:
    digitalWrite(segA, segmento_ligado);
    digitalWrite(segB, segmento_ligado);
    digitalWrite(segC, segmento_ligado);
    digitalWrite(segD, segmento_ligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_ligado);
    digitalWrite(segG, segmento_ligado);
    break;

  case 10:
    digitalWrite(segA, segmento_desligado);
    digitalWrite(segB, segmento_desligado);
    digitalWrite(segC, segmento_desligado);
    digitalWrite(segD, segmento_desligado);
    digitalWrite(segE, segmento_desligado);
    digitalWrite(segF, segmento_desligado);
    digitalWrite(segG, segmento_desligado);
    break;
  }
}