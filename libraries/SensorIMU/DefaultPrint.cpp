/*
  DefaultPrint.cpp - Library for printing data in a default way.
  Created by Rafael Araujo Lehmkuhl, April 11, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "DefaultPrint.h"

DefaultPrint Data;

void DefaultPrint::print_triple(double x, double y, double z) 
{
  // Mostra os valores no serial monitor
  //Serial.print("\t");
  //Serial.print("X:");
  Serial.print(x);
  Serial.print(";");

  //Serial.print("   ");
  //Serial.print("Y:");
  Serial.print(y);
  Serial.print(";");

  //Serial.print("   ");
  //Serial.print("Z:");
  Serial.print(z);
  Serial.print(";");
  
  Serial.println();
}

void DefaultPrint::print_single(double x) 
{
  // Mostra os valores no serial monitor
  //Serial.print("\t");
  //Serial.print("X:");
  Serial.print(x);
  Serial.print(";");

  Serial.println();
}