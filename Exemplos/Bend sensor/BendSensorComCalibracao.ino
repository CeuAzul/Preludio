#include <SensorBend.h>
#include <Csuporte.h>

#define AP_BENDSENSPIN A0       //Pino do Bend sensor


SensorBend bend(AP_BENDSENSPIN);



void setup(){
  Serial.begin(115200);
  char junk = ' ';

  // Calibração --------------------------------------------------------------------------------

  Serial.println("Coloque o bend em um grau grande (60 graus por exemplo) e digite a angulacao");
  bend.calibraComoMaximo(getFloatFromSerialMonitor());
  Serial.println("Grau maximo calibrado!"); 


  Serial.println("Coloque o bend em um grau negativo grande (-60 graus por exemplo) e digite a angulacao");
  bend.calibraComoMinimo(getFloatFromSerialMonitor());
  Serial.println("Grau minimo calibrado!"); 

  Serial.println("Coloque o bend na posicao zero e digite qualquer coisa");
  Serial.flush(); //flush all previous received and transmitted data
  while(!Serial.available()) ; // hang program until a byte is received notice the ; after the while()
  bend.calibraComoZero();
  Serial.println("Grau zero calibrado!"); 



}

void loop(){
    bend.atualizaBend();                
    
    Serial.print(bend.getGrauBendRaw());
    Serial.print(",");
    Serial.print(bend.getGrauBendMedia());
    Serial.print(",");
    Serial.print(bend.getGrauBend());
    Serial.print("----------/  ");
    Serial.print(bend.getCalibrAnalogMax());
    Serial.print(",");
    Serial.print(bend.getCalibrAnalogMin());
    Serial.print(",");
    Serial.println(bend.getCalibrAnalogZero());

    delay(20);
}


float getFloatFromSerialMonitor(){
  char inData[20];  
  float f=0;    
  int x=0;  
  while (x<1){  
      String str;   
      if (Serial.available()) {
        delay(100);
        int i=0;
        while (Serial.available() > 0) {
         char  inByte = Serial.read();
          str=str+inByte;
          inData[i]=inByte;
          i+=1;
          x=2;
        }
        f = atof(inData);
        memset(inData, 0, sizeof(inData));  
      }
    }//END WHILE X<1  
    return f; 
  }