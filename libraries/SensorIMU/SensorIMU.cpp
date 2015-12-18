// Inclui as bibliotecas necessárias
#include <Wire.h>
#include <MatrixM.h>           // Biblioteca para operações com matrizes
#include <VectorM.h>           // Biblioteca para operações com vetores
#include <I2CRW.h>             // Biblioteca com funções padronizadas de leitura e escrita em registradores através de interface I2C
#include <IMU.h>               // Biblioteca para recebimento dos dados da IMU
#include <GY86.h>              // Biblioteca com funções padronizadas da GY80
#include <DCMPI.h>             // Biblioteca com código do controlador PI(D)
#include <SORT.h>              // Biblioteca para obter a mediana de 3 dados
#include <DefaultPrint.h>      // Biblioteca para a padronização de escrita de dados
#include <MS5611.h>
#include "Arduino.h"
#include <SensorIMU.h>



double Xg, Yg, Zg;             // Variáveis para os três eixos do giroscópio
double Xa, Ya, Za;             // Variáveis para os três eixos do acelerômetro
double cogX, cogY;             // Variáveis para a direção de voo pelo GPS

double time=0;                 // Variável para a marcação do tempo atual
double timePassed=0;           // Variável para a marcação do tempo entre dois loops
  
double dx, dy, dz;             // Ângulos de rotação entre duas aquisições de dados
double Wx, Wy, Wz;             // Taxas de rotação entre duas aquisições de dados                            

// Vetor acelerômetro                         
double  acc_vector[3] = {0, 0, 0};



// Ângulo de direção de voo do GPS  
double  cog;
// Erro em YAW no plano da Terra
double YcoG;

double W_Ico[3] = {0,0,0};
double W_co[3] =  {0,0,0};

double yaw = 0;
double pitch = 0;
double roll = 0;

// Matriz DCM
double  oldDCM[3][3] = {{0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0}};

// Matriz DCM temporária
double  newDCM[3][3] = {{1, 0, 0},
                        {0, 1, 0},
                        {0, 0, 1}};

// Matriz de giro
double  rotation_matrix[3][3] = {{1, 0, 0},
                                 {0, 1, 0},
                                 {0, 0, 1}};

double W_RP = 10.0;     // Peso da correção em Roll & Pitch
double W_Y  = 0.0;     // Peso da correção em Yaw
double K_P  = 1.0;     // Ponderador Proporcional
double K_I  = 1.0;     // Ponderador Integral


// Variáveis para o barômetro -------------
MS5611 ms5611;

double referencePressure;

double realTemperature;
long realPressure;
float absoluteAltitude;
float relativeAltitude;
float airDensity;
float pressaoAtmosfericaPadrao = 101320; // Utilizado para o cálculo da altitude pressão
bool bar400Hz = true;
// ----------------------------------------
int nMedias = 1;
bool mpu400Hz = true;


void SensorIMU::inicializaIMU(int numMediasFiltro, bool mpu400hz, bool barom400Hz, bool boaPrecisaoBarometro){
 // Serial.println("iniciou");
  Unit.inicializaIMURaw(numMediasFiltro, mpu400hz);
// Serial.println("Calibrou IMU raw");
  Device.setupGYR(500);    // Inicia o Giroscópio
//  Serial.println("Setou gyro");
  Device.setupACC(8);      // Inicia o Acelerômetro
//  Serial.println("Setou acc");
  Device.calibrateGYR(100);  // Calibra o Giroscópio
//  Serial.println("calibrou gyro");
  Device.calibrateACC(100);  // Calibra o Acelerômetro
//  Serial.println("calibrou acc");
  delay(1000);                   // Aguarda a resposta do sensor


// -----------   BAROMETRO --------------------------
  //Inicia e calibra o módulo MS5611
  setupMS5611(barom400Hz, boaPrecisaoBarometro);
  calibrateMS5611();
// --------------------------------------------------
  // Aguarda a resposta do sensor
  delay(1500);
}


void SensorIMU::atualizaIMU(float curse){
  //long tempoteste = micros();
  // Atualiza os valores do gyro e do acelerometro
  Device.atualizaValoresCalibrados();
 // tempoteste = micros() - tempoteste;
 // relativeAltitude = tempoteste;

  Xg = Device.getGyXCalibrado();
  Yg = Device.getGyYCalibrado();
  Zg = Device.getGyZCalibrado();
  Xa = Device.getAcXCalibrado();
  Ya = Device.getAcYCalibrado();
  Za = Device.getAcZCalibrado();
  realTemperature = Device.getTempCalibrada();

  // Atualiza os valores de direção do GPS
  cog = 0; //Atualiza valor do cog pelo parâmetro de atualização da função
  
  Matrix.equal(oldDCM, newDCM);   // Matriz oldDCM recebe o valor da matriz newDCM (DCM anterior)
  Matrix.zero(newDCM);            // Zera a matriz newDCM




  timePassed = (millis() - time)/1000;     // Tempo passado desde o último loop é a diferença entre o tempo atual e o último valor gravado
  time=millis();                           // Recebe o valor atual do tempo para usar como comparação no próximo loop
  
  // Passa os valores recebidos no Acelerômetro pro Vetor Acelerômetro
  acc_vector[0] = Xa;
  acc_vector[1] = Ya;
  acc_vector[2] = Za;
  Vector.normalize(acc_vector);            // Normaliza o Vetor Acelerômetro para que este tenha comprimento igual a unidade
  
  // Passa os valores recebidos no Giroscópio para o Vetor Giroscópio
  Wx = Xg - W_co[0];
  Wy = Yg - W_co[1];
  Wz = 0;//Zg - W_co[2]; //Comentado porque ta sem controle de YAW
  
  // Passa os valores recebidos no GPS pro Vetor GPS
  cogX = cos(cog);
  cogY = sin(cog);
  YcoG = 0;//oldDCM[0][0] * cogY - oldDCM[1][0] * cogX;

  // Multiplica a taxa de rotação instantânea W em cada eixo pelo tempo passado desde o último loop, gerando ângulos de rotação
  dx = Wx*timePassed;
  dy = Wy*timePassed;
  dz = Wz*timePassed;
  
  // Atualiza matriz de rotação
  rotation_matrix[0][0] =   1;
  rotation_matrix[0][1] = -dz;
  rotation_matrix[0][2] =  dy;
  rotation_matrix[1][0] =  dz;
  rotation_matrix[1][1] =   1;
  rotation_matrix[1][2] = -dx;
  rotation_matrix[2][0] = -dy;
  rotation_matrix[2][1] =  dx;
  rotation_matrix[2][2] =   1;
  
  // "Mescla" os valores do magnetômetro e do acelerômetro aos do giroscópio (matriz oldDCM), através de um controlador PI
  DCMinstance.PI_controller(oldDCM, YcoG, acc_vector, timePassed, W_Ico, W_co, W_RP, W_Y, K_P, K_I);


  Matrix.multiply(newDCM, rotation_matrix, oldDCM);   // Multiplica a matriz de rotação (rotation_matrix) pela antiga matriz DCM (oldDCM), gerando a nova matriz DCM (newDCM)
  Matrix.orthogonalize(newDCM);                       // Ortogonaliza a nova matriz DCM para que os tres eixos estejam a 90 graus uns dos outros
  Matrix.normalize(newDCM);                           // Normaliza a nova matriz DCM para que os eixos tenham comprimento igual a unidade
  
  // Calcula os ângulos de Yaw, Pitch & Roll
  yaw =   (atan2(newDCM[1][0],newDCM[0][0]))*180.0/PI;
  pitch = (asin(newDCM[2][0]))*180.0/PI;
  roll =   (atan2(newDCM[2][1],newDCM[2][2]))*180.0/PI;



  atualizaBarometro();
  
  //Data.print_triple(yaw, pitch, roll);
  //Matrix.printData(newDCM);
  //Data.print_triple(acc_vector[0], acc_vector[1], acc_vector[2]);

  //Nomes das variáveis:

  // Dados do Giroscópio:       Xg, Yg, Zg
  // Dados do Acelerômetro:     Xa, Ya, Za
  // Ângulos:                   yaw, pitch, roll


}

double SensorIMU::getXg(){
  return Xg;
}

double SensorIMU::getYg(){
  return Yg;
}

double SensorIMU::getZg(){
  return Zg;
}

double SensorIMU::getXa(){
  return Xa;
}

double SensorIMU::getYa(){
  return Ya;
}

double SensorIMU::getZa(){
  return Za;
}

double SensorIMU::getYaw(){
  return yaw;
}

double SensorIMU::getPitch(){
  return pitch;
}

double SensorIMU::getRoll(){
  return roll;
}


void SensorIMU::setCurse(double curse){
  cog = curse;
}

// Funções relacionadas ao barômetro da INU

void SensorIMU::atualizaBarometro(){

  // Read raw values
 // uint32_t rawTemp = ms5611.readRawTemperature();
 // uint32_t rawPressure = ms5611.readRawPressure();

  // Read true temperature & Pressure
  realTemperature = ms5611.readTemperature();
  realPressure = ms5611.readPressure();

  // Calculate altitude
  absoluteAltitude = ms5611.getAltitude(realPressure, pressaoAtmosfericaPadrao);
  relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);

}


void SensorIMU::setupMS5611(bool i2c400Hz, bool preciso){
  if(!preciso){
    ms5611.begin(i2c400Hz);
  }else{
    ms5611.begin(i2c400Hz, MS5611_HIGH_RES);    
  }
}

void SensorIMU::calibrateMS5611(){
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

}

double SensorIMU::getTemperatura(){
  return realTemperature;
}

long SensorIMU::getPressao(){
  return realPressure;
}

float SensorIMU::getAbsAltitude(){
  return absoluteAltitude;
}

float SensorIMU::getRelAltitude(){
  return relativeAltitude;
}

float SensorIMU::getDensidadeAr(){
  return  (realPressure/(287.05*(realTemperature+273.15))) ;
}