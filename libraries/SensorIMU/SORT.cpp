/*
  SORT.cpp - Library for sorting a set of 3 numbers.
  Created by Rafael Araujo Lehmkuhl, March 29, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SORT.h"

SORT SORTED;

void SORT::sort(double a[]) 
{
  for(int i=0; i<2; i++) 
  {
    for(int o=0; o<(3-(i+1)); o++) 
    {
      if(a[o] > a[o+1]) 
      {
        int t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;
      }
    }
  }
}