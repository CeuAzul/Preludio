
/*
  SensorPwm - Biblioteca de funções relacionadas ao sensore PWM dos servos e (nessa versão) ao bend sensor
*/

#include "Arduino.h"
#include "SensorPWM.h"
#include <Csuporte.h>


using namespace std;

byte pin[] = {0, 0, 0, 0};    //for maximum efficency thise pins should be attached

volatile int CANAL_1_PINO;
volatile int CANAL_2_PINO;
volatile int CANAL_3_PINO;
volatile int CANAL_4_PINO;
volatile int VRPM_PINO;


const int GRAUMAX = 0;
const int PWM0 = 1;
const int PWMMAX = 2;

const int TMIN = 700;
const int TMAX = 2700;

const float KV = 250;                    // Kv = constante que relaciona tensão e Rpm
const float EFF = 0.95;                  // Eff é a relação entre o valor de Kv real e Kv ideal
const float VMAX = 50;                   // Tensão máxima do motor
const float RINTERNA = 0.037;            // Resistencia interna do motor
const float PMAX = 2700;                 // Potência máxima entregue (utilizada para mapear o PWM)
float PMaxPWM = 1800;                    // PWM máximo do motor (Valor do pwm quando levamos o botão do controle lá pra cima)
float PZeroPWM = 1000;                   // PWM zero, (Valor do PWM quando o motor ta desligado)
float rpmVoltage;
float rawVoltage;
float potenciaMotor;

float calibr1[3];                        // Vetores com dados para calibrar as superfícies
float calibr2[3];                        // [grauMax, grauMin, pwm0, pwmMax, pwmMin] <-- esses 5 dados que precisam
float calibr3[3];
float calibr4[3];

bool modoRPM = false;

float grauPWM1, grauPWM2, grauPWM3;

long grauPWM4;

float ch1, ch2, ch3, ch4;
Csuporte sup;


//this is all normal arduino stuff
SensorPwm::SensorPwm(int canal1pin, int canal2pin, int canal3pin, int canal4pin){
  //  Serial.begin(9600);
    pin[0] = canal1pin;
    pin[1] = canal2pin;
    pin[2] = canal3pin;
    pin[3] = canal4pin;

    for (byte i=0; i<4; i++){
        pinMode(pin[i], INPUT);     //set the pin to input
        digitalWrite(pin[i], HIGH); //use the internal pullup resistor
    }

    
    calibr1[GRAUMAX] = 11;            //Calibração do canal 1 para plataforma de testes
    calibr1[PWM0] = 1385;
    calibr1[PWMMAX] = 1826;
    
    calibr2[GRAUMAX] = 11;            //Calibração do canal 2
    calibr2[PWM0] = 1370;
    calibr2[PWMMAX] = 1826;
    
    calibr3[GRAUMAX] = 11;            //Calibração do canal 3
    calibr3[PWM0] = 1370;             //Colocar aqui o valor que vem direto do sensor, quando o ângulo for 0 graus
    calibr3[PWMMAX] = 1826;           //Colocar aqui o valor que vem direto do sensor quando o angulo for GRAUMAX
    
    calibr4[GRAUMAX] = 11;            //Calibração do canal 4
    calibr4[PWM0] = 1370;     
    calibr4[PWMMAX] = 1826;       
    
    CANAL_1_PINO = canal1pin;         //Conecta dados vindo da chamada da função
    CANAL_2_PINO = canal2pin;
    CANAL_3_PINO = canal3pin;
    CANAL_4_PINO = canal4pin;

}

void SensorPwm::atualizaPWM(long valor, int vez){

     switch (vez) {

      case 0:

        break;





      case 1:
      // PWM SETTINGS CH2 ---------------------------------------------------------

        break;





      case 2:
        // PWM SETTINGS CH3 ---------------------------------------------------------

        break;






      case 3:
        if(!modoRPM){  // MODO SERVO  ---------------------------------------------------------
          ch4 = valor;
          if((ch4 > TMIN) && (ch4 < TMAX)){     // Verifica se o tempo é válido
            if(calibr4[PWMMAX] >= calibr4[PWM0]){
               grauPWM4 =  sup.mapfloat(ch4, calibr4[PWM0], calibr4[PWMMAX], 0, 11500);
            }else{
               grauPWM4 =  sup.mapfloat(ch4, calibr4[PWM0], calibr4[PWMMAX], 0, (-1*11500));
               grauPWM4 = -1*grauPWM4;
            }

          }else{
            ch4 = -999;
            grauPWM4 = 0;
          }

        }else{        // MODO RPM  ---------------------------------------------------------

        // RPM Settings ---------------------------------------------------------
          ch4 = valor;
          rawVoltage = analogRead(VRPM_PINO);
        // Le a a tensão relacionada do motor
          rpmVoltage = sup.mapfloat(rawVoltage, 0, 1024, 0, VMAX);

        // Mapeia Potencia transmitida pro motor entre os valores do PWM
          potenciaMotor =  sup.mapfloat(ch4, PZeroPWM, PMaxPWM, 0, PMAX);

        }

        break;


      default:
        break;

        }
  

}





void SensorPwm::pegaAngulo1(long valor){
          ch1 = valor;
              // PWM SETTINGS CH1 ---------------------------------------------------------
        if((ch1 > TMIN) && (ch1 < TMAX)){     // Verifica se o tempo é válido
          if(calibr1[PWMMAX] > calibr1[PWM0]){
             grauPWM1 =  sup.mapfloat(ch1, calibr1[PWM0], calibr1[PWMMAX], 0, calibr1[GRAUMAX]);
          }else{
             grauPWM1 =  sup.mapfloat(ch1, calibr1[PWM0], calibr1[PWMMAX], 0, (-1*calibr1[GRAUMAX]));
             grauPWM1 = -1*grauPWM1;
          }
        }else{
          ch1 = -999;
          grauPWM1 = 0;
        }

}

void SensorPwm::pegaAngulo2(long valor){
        ch2 = valor;
        if((ch2 > TMIN) && (ch2 < TMAX)){     // Verifica se o tempo é válido
          if(calibr2[PWMMAX] > calibr2[PWM0]){
             grauPWM2 =  sup.mapfloat(ch2, calibr2[PWM0], calibr2[PWMMAX], 0, calibr2[GRAUMAX]);
          }else{
             grauPWM2 =  sup.mapfloat(ch2, calibr2[PWM0], calibr2[PWMMAX], 0, (-1*calibr2[GRAUMAX]));
             grauPWM2 = -1*grauPWM2;
          }
        }else{
          ch2 = -999;
          grauPWM2 = 0;
        }

}

void SensorPwm::pegaAngulo3(long valor){
        ch3 = valor;
        if((ch3 > TMIN) && (ch3 < TMAX)){     // Verifica se o tempo é válido
          if(calibr3[PWMMAX] > calibr3[PWM0]){
             grauPWM3 =  sup.mapfloat(ch3, calibr3[PWM0], calibr3[PWMMAX], 0, calibr3[GRAUMAX]);
          }else{
             grauPWM3 =  sup.mapfloat(ch3, calibr3[PWM0], calibr3[PWMMAX], 0, (-1*calibr3[GRAUMAX]));
             grauPWM3 = -1*grauPWM3;
          }
        }else{
          ch3 = -999;
          grauPWM3 = 0;
        }

}

void SensorPwm::pegaAngulo4(long valor){
        ch4 = valor;
        if((ch4 > TMIN) && (ch4 < TMAX)){     // Verifica se o tempo é válido
          if(calibr4[PWMMAX] >= calibr4[PWM0]){
             grauPWM4 =  map(ch4, calibr4[PWM0], calibr4[PWMMAX], 0, 11500);
          }else{
             grauPWM4 =  map(ch4, calibr4[PWM0], calibr4[PWMMAX], 0, (-1*11500));
             grauPWM4 = -1*grauPWM4;
          }

        }else{
          ch4 = -999;
          grauPWM4 = 0;
        }

        if((grauPWM4 < 0)){
          grauPWM4 = 0;
        }
        if(grauPWM4 > 11500){
          grauPWM4 = 11500;
        }

}













// Modo de operação --------------------------------------------------------------
// Escolhendo o modo de operação 
void SensorPwm::setModoRPM(int pinoVRPM){
  pinMode(pinoVRPM, INPUT);        // Input para o RPM
  VRPM_PINO = pinoVRPM; 
  modoRPM = true;
}



// Getters-----------------------------------------------------------------------------

float SensorPwm::getGrauPWM1(){
  return grauPWM1;
}

float SensorPwm::getGrauPWM2(){
  return grauPWM2;
}

float SensorPwm::getGrauPWM3(){
  return grauPWM3;
}

long SensorPwm::getGrauPWM4(){
  return grauPWM4;
}

float SensorPwm::getRPM(){                  
  return (EFF*KV*(rpmVoltage - RINTERNA*potenciaMotor/rpmVoltage));        
}  

float SensorPwm::getPotenciaDeEntrada(){           
  return potenciaMotor;                  
}

float SensorPwm::getCorrenteUsada(){           
  return (potenciaMotor/rpmVoltage);                  
}

float SensorPwm::getTensaoMotor(){           
  return rpmVoltage;                  
}

float SensorPwm::getRawVoltage(){           
  return rawVoltage;                  
}

/*
O RPM é calculado da seguinte forma.

Rpm = Kv( Um - Ri * Imot )

Onde:
Kv = constante que relaciona tensão e Rpm
Um = tensão no motor (entrada analógica)
Ri = resistência interna do motor
Imot = corrente sendo gasta

Como não temos Imot diretamente, podemos encontrar pela potência passada pelo pwm, onde

Imot = Pin/Umot 

onde Pin é a potência de entrada do motor. (Potência mapeada pelo pwm).

Substituindo na equação vamos ter:

Rpm = Eff * Kv *[ Umot - Ri * Pin/Umot ]

(Eff é a relação entre o valor de Kv real e Kv ideal)

Pelos dados do fabricante do motor:
Ri = 0,037 ohm
Pmax = 2700 W
Kv = 250 Rpm/Volt
*/







float SensorPwm::getPwm4Raw(){
  return ch4;
}

float SensorPwm::getPwm3Raw(){
  return ch3;
}

float SensorPwm::getPwm2Raw(){
  return ch2;
}

float SensorPwm::getPwm1Raw(){
  return ch1;
}

void SensorPwm::calibraPWM(int nPWM, int pwmZero, int pwmMaximo, int grauMaximo){
  switch (nPWM) {
    case 1:
      calibr1[PWM0] = pwmZero;
      calibr1[PWMMAX] = pwmMaximo;
      calibr1[GRAUMAX] = grauMaximo;
      break;
    case 2:
      calibr2[PWM0] = pwmZero;
      calibr2[PWMMAX] = pwmMaximo;
      calibr2[GRAUMAX] = grauMaximo;
      break;
    case 3:
      calibr3[PWM0] = pwmZero;
      calibr3[PWMMAX] = pwmMaximo;
      calibr3[GRAUMAX] = grauMaximo;
      break;
    case 4:
      calibr4[PWM0] = pwmZero;
      calibr4[PWMMAX] = pwmMaximo;
      calibr4[GRAUMAX] = grauMaximo;
      break;
    default: 
    break;
  }
}

