#ifndef Display7seg_h
#define Display7seg_h

#include "Arduino.h"

class Display7seg{
	public:
		Display7seg(int digit11=23, int segA1=25, int segF1=27, int digit21=29, int digit31=31, int segB1=33, int segE1=35, int segD1=37, int pto = 39, int segC1=41,  int segG1=43, int digit41=45);
		void mostraNumero(int toDisplay);
	private:
		void iluminarnumero(int numberToDisplay);
};
#endif