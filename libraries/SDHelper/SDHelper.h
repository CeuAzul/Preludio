#ifndef SDHelper_h
#define SDHelper_h

#include "Arduino.h"
#include <SdFat.h>

class SDHelper{
  public:
    SDHelper(int p = 53); // 53 no mega e 4 no uno

    void escreveDadoSD(float dado, int size);
    void escreveDadoSD(int dado);
    void escreveDadoSD(long dado);
    void escreveDadoSD(unsigned long dado);
    void escreveDadoSD(String dado);
    void escreveDadoSD(double dado);
    void escreveNovaLinhaSD();  
    void abreSD();  
    void ligaSD();  
    void finalizaSD();  
    bool leConfigSD();
    bool temArquivoSD();
    void calibraUltrassomNoSD(int ultrassomDist);
    void calibraPWMNoSD(int pwmNum, int raw0, int rawMax, int grauMax);
    void escreveConfigSD(int ultrassomDist, int pwm1Raw0, int pwm1RawMax, int pwm1GrauMax, int pwm2Raw0, int pwm2RawMax, int pwm2GrauMax, int pwm3Raw0, int pwm3RawMax, int pwm3GrauMax, int pwm4Raw0, int pwm4RawMax, int pwm4GrauMax);
    int getUltrassomDist();
    int getPwm1Raw0();
    int getPwm1RawMax();
    int getPwm1GrauMax();
    int getPwm2Raw0();
    int getPwm2RawMax();
    int getPwm2GrauMax();
    int getPwm3Raw0();
    int getPwm3RawMax();
    int getPwm3GrauMax();
    int getPwm4Raw0();
    int getPwm4RawMax();
    int getPwm4GrauMax();


  private:
    void applySetting(String settingName, String settingValue);
    float toFloat(String settingValue);
    long toLong(String settingValue);
    bool toBoolean(String settingValue);

};





#endif
