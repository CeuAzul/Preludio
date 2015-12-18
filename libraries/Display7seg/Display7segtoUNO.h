#ifndef Display7seg_h
#define Display7seg_h

#include "Arduino.h"

class Display7seg
{
	public:
		Display7seg(int segA1=2, int segB1=3, int segC1=4, int segD1=5, int segE1=6, int segF1=7, int segG1=8, int digit11=A3, int digit21=A2, int digit31=A1, int digit41=A0);
		void mostrarnumero(int toDisplay);
	private:
		void iluminarnumero(int numberToDisplay);
};
#endif