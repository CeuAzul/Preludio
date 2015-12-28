#include <SDHelper.h>
#include <SensorPwm.h>
#include <MenuDeCalibracao.h>
#include <SDHelper.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

#include <Csuporte.h>

#define DP_CHANNEL_1_PIN 2  
#define CHANNEL_1_INT 0                                     
#define DP_CHANNEL_2_PIN 3  
#define CHANNEL_2_INT 1                                  
#define DP_CHANNEL_3_PIN 18 
#define CHANNEL_3_INT 5                                       
#define DP_CHANNEL_4_PIN 19  
#define CHANNEL_4_INT 4

SensorPwm pwm(DP_CHANNEL_1_PIN, DP_CHANNEL_2_PIN, DP_CHANNEL_3_PIN, DP_CHANNEL_4_PIN);

SDHelper sdhelp;

void setup(){
    MenuDeCalibracao::iniciaMenuDeCalibracao(8);
    Serial.begin(9600);


    bool temArquivo = sdhelp.leConfigSD();                  // Verifica se tem arquivo de calibração
    if(temArquivo){
        if(sdhelp.getPwm1Raw0() != -9999){
            pwm.calibraPWM(1, sdhelp.getPwm1Raw0(), sdhelp.getPwm1RawMax(), sdhelp.getPwm1GrauMax());
            Serial.println("Calibrou PWM1");        
        }
        if(sdhelp.getPwm2Raw0() != -9999){
            pwm.calibraPWM(2, sdhelp.getPwm2Raw0(), sdhelp.getPwm2RawMax(), sdhelp.getPwm2GrauMax());
            Serial.println("Calibrou PWM2");        
        }
        if(sdhelp.getPwm3Raw0() != -9999){
            pwm.calibraPWM(3, sdhelp.getPwm3Raw0(), sdhelp.getPwm3RawMax(), sdhelp.getPwm3GrauMax());
            Serial.println("Calibrou PWM3");        
        }
        if(sdhelp.getPwm4Raw0() != -9999){
            pwm.calibraPWM(4, sdhelp.getPwm4Raw0(), sdhelp.getPwm4RawMax(), sdhelp.getPwm4GrauMax());
            Serial.println("Calibrou PWM4");        
        }
    }else{
        Serial.println("Não encontrou arquivo de calibração");
    }
}

void loop(){
    pwm.atualizaPWM();
    if(MenuDeCalibracao::lcdLigado()){
        MenuDeCalibracao::atualizaMenuDados();
    //  MenuDeCalibracao::setValorUltrassomRaw(10);

        MenuDeCalibracao::setValorPWMRaw(pwm.getPwm1Raw(), pwm.getPwm2Raw(), pwm.getPwm3Raw(), pwm.getPwm4Raw());
        String oi[8] = {String((int) (pwm.getGrauPWM1()*10)),
                        String((int) (pwm.getGrauPWM2()*10)),
                        String((int) (pwm.getGrauPWM3()*10)),
                        String((int) (pwm.getGrauPWM4()*10)),
                        String((int) (pwm.getPwm3Raw())),
                        String((int) (pwm.getPwm2Raw())),
                        String((int) (pwm.getPwm3Raw())),
                        String((int) (pwm.getPwm4Raw())),   };
        MenuDeCalibracao::setDadosParaExibir(oi);
    }

    if(MenuDeCalibracao::emModoLeitura()){
        Serial.print(pwm.getGrauPWM1());
        Serial.print(", ");
        Serial.print(pwm.getGrauPWM2());
        Serial.print(", ");
        Serial.print(pwm.getGrauPWM3());
        Serial.print(", ");
        Serial.print(pwm.getGrauPWM4());
        Serial.print(", ");
        Serial.print(pwm.getPwm1Raw());
        Serial.print(", ");
        Serial.print(pwm.getPwm2Raw());
        Serial.print(", ");
        Serial.print(pwm.getPwm3Raw());
        Serial.print(", ");
        Serial.print(pwm.getPwm4Raw());
        Serial.println(", ");
    }
    delay(150);


}