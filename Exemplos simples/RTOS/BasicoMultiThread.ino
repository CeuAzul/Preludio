// Teste de funções utilizando o sistema operacional

#include <NilRTOS.h>
#include <NilSerial.h>


#define Serial NilSerial						// Redefine serial para usar do sistema operacional

NIL_WORKING_AREA(waThread1, 64);				// Declara e reserva um espaço para a area de trabalho da thread

NIL_THREAD(Thread1, arg){
	long tempo;
	while(true){
		Serial.print("1");
        nilThdSleep(30); 
	}
}

NIL_WORKING_AREA(waThread2, 64);				// Declara area de trabalho e reserva espaço

NIL_THREAD(Thread2, arg) {
  long tempo;	
  while (true) {
  	Serial.print("2");
    nilThdSleep(20);
  }
}

NIL_WORKING_AREA(waThread3, 64);

NIL_THREAD(Thread3, arg) {
  long tempo;
  while (true) {
  	Serial.print("3");
    nilThdSleep(10);
   
  }
}

NIL_WORKING_AREA(waThread4, 64);

NIL_THREAD(Thread4, arg) {
  long tempo;
  while (true) {
    Serial.print("4");
   
  }
}

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, waThread3, sizeof(waThread3))
NIL_THREADS_TABLE_ENTRY(NULL, Thread4, NULL, waThread4, sizeof(waThread4))
NIL_THREADS_TABLE_END()


void setup() {

  Serial.begin(9600);
  // start kernel
  nilSysBegin();
}


void loop() {
    Serial.print("-");
}