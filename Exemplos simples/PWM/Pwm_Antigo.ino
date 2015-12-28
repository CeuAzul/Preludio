#include <SensorPwm.h>
#include <Csuporte.h>

#define AP_VRPMPIN A0           //Pino do RPM

#define DP_CHANNEL_1_PIN 2  
#define CHANNEL_1_INT 0 
                                                           
                                           
#define DP_CHANNEL_2_PIN 3  
#define CHANNEL_2_INT 1
                                       
#define DP_CHANNEL_3_PIN 18 
#define CHANNEL_3_INT 5

                                          
#define DP_CHANNEL_4_PIN 19  
#define CHANNEL_4_INT 4




//this is all normal arduino stuff
void setup(){
    attachInterrupt(CHANNEL_1_INT, calcSignalCh1, CHANGE);    //INT-4 AILERON
    attachInterrupt(CHANNEL_2_INT, calcSignalCh2, CHANGE);    //INT-5 Freio
    attachInterrupt(CHANNEL_3_INT, calcSignalCh3, CHANGE);    //INT-3 AILERON
    attachInterrupt(CHANNEL_4_INT, calcSignalCh4, CHANGE);    //INT-2 FREIO
   
  //  pwm.setModoRPM(AP_VRPMPIN);
    Serial.begin(115200);
}

void loop(){
    pwm.atualizaPWM();                //Aqui o dado vai estar alocado na variavel dados
    
    Serial.print(pwm.getGrauPWM1());
    Serial.print(",   ");
    Serial.print(pwm.getGrauPWM2());
    Serial.print(",   ");
    Serial.print(pwm.getGrauPWM3());
    Serial.print(",   ");
    Serial.print(pwm.getGrauPWM4());
    Serial.print(",   ");
    Serial.print(pwm.getRPM());
    Serial.print(",");
    Serial.print(pwm.getTensaoMotor());
    Serial.print(",");
    Serial.print(pwm.getPotenciaDeEntrada());
    Serial.print(",");
    Serial.print(pwm.getCorrenteUsada());
    Serial.print(",");
    Serial.println(pwm.getRawVoltage());
    delay(100);
}

void calcSignalCh1(){                     
  pwm.sinalCH1();
}

void calcSignalCh2(){                       
  pwm.sinalCH2();
}

void calcSignalCh3(){                 
  pwm.sinalCH3();
}

void calcSignalCh4(){                   
  pwm.sinalCH4();
}
