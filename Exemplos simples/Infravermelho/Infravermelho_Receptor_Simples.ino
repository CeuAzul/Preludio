/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

const long  IR_0		=	0x20DF08F7;
const long  IR_1		=	0x20DF8877;
const long  IR_2		=	0x20DF48B7;
const long  IR_3		=	0x20DFC837;
const long  IR_4		=	0x20DF28D7;
const long  IR_5		=	0x20DFA857;
const long  IR_6		=	0x20DF6897;
const long  IR_7		=	0x20DFE817;
const long  IR_8		=	0x20DF18E7;
const long  IR_9		=	0x20DF9867;
const long  IR_OK		=	0x20DF22DD;
const long  IR_CIMA		=	0x20DF02FD;
const long  IR_BAIXO	=	0x20DF827D;
const long  IR_ESQUERDA	=	0x20DFE01F;
const long  IR_DIREITA	=	0x20DF609F;
const long  IR_LIGA		=	0x20DF10EF;
const long  IR_MENOS	=	0x20DF32CD;
const long  IR_PLAY		=	0x20DF0DF2;
const long  IR_PAUSE	=	0x20DF5DA2;
const long  IR_STOP		=	0x20DF8D72;
const long  IR_VOLTA	=	0x20DF14EB;

long armazenavalor;
int RECV_PIN = 7;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
if (irrecv.decode(&results))  {  
 //   Serial.print("Valor lido : ");  
 //   Serial.println(results.value, HEX);  
    armazenavalor = (results.value);  
    switch (armazenavalor) {
        case IR_0:
          Serial.println("0"); 
          break;
        case IR_1:
          Serial.println("1"); 
          break;
        case IR_2:
          Serial.println("2"); 
          break;
        case IR_3:
          Serial.println("3"); 
          break;
        case IR_4:
          Serial.println("4"); 
          break;
        case IR_5:
          Serial.println("5"); 
          break;
        case IR_6:
          Serial.println("6"); 
          break;
        case IR_7:
          Serial.println("7"); 
          break;
        case IR_8:
          Serial.println("8"); 
          break;
        case IR_9:
          Serial.println("9"); 
          break;
        case IR_OK:
          Serial.println("OK"); 
          break;
        case IR_CIMA:
          Serial.println("CIMA"); 
          break;
        case IR_BAIXO:
          Serial.println("BAIXO"); 
          break;
        case IR_ESQUERDA:
          Serial.println("ESQUERDA"); 
          break;
        case IR_DIREITA:
          Serial.println("DIREITA"); 
          break;
        case IR_LIGA:
          Serial.println("LIGA"); 
          break;
        case IR_MENOS:
          Serial.println("MENOS"); 
          break;
        case IR_PLAY:
          Serial.println("PLAY"); 
          break;
        case IR_PAUSE:
          Serial.println("PAUSE"); 
          break;
        case IR_STOP:
          Serial.println("STOP"); 
          break;
        case IR_VOLTA:
          Serial.println("VOLTA"); 
          break;
        default:
        	break;
        //  Serial.println("merda"); 
    }

  irrecv.resume(); //Le o próximo valor  
  }  





  delay(100);
}

