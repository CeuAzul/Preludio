/*
  Header da IMU
*/
#ifndef SensorIMU_h
#define SensorIMU_h

#include <Arduino.h>
#include <Wire.h>             // Biblioteca de comunicação I2C
#include <MatrixM.h>           // Biblioteca para operações com matrizes
#include <VectorM.h>           // Biblioteca para operações com vetores
#include <I2CRW.h>             // Biblioteca com funções padronizadas de leitura e escrita em registradores através de interface I2C
#include <IMU.h>               // Biblioteca para recebimento dos dados da IMU
#include <GY86.h>              // Biblioteca com funções padronizadas da GY80
#include <DCMPI.h>             // Biblioteca com código do controlador PI(D)
#include <SORT.h>              // Biblioteca para obter a mediana de 3 dados
#include <DefaultPrint.h>      // Biblioteca para a padronização de escrita de dados
#include <MS5611.h>

class SensorIMU{
  public:
    void inicializaIMU(int numMediasFiltro = 4, bool mpu400hz = false, bool barom400Hz = false, bool boaPrecisaoBarometro = false);
    void atualizaIMU(float curse = 0);
    double getXg();
    double getYg();
    double getZg();
    double getXa();
    double getYa();
    double getZa();
    double getYaw();
    double getPitch();
    double getRoll();
    void setCurse(double curse);
    //Barômetro:
    double getTemperatura();
    long getPressao();
    float getAbsAltitude();
    float getRelAltitude();
    float getDensidadeAr();


  private:
    void atualizaBarometro();
    void setupMS5611(bool i2c400Hz, bool preciso);
    void calibrateMS5611();
    
};




#endif
