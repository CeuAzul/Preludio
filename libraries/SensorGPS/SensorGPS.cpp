#include <Csuporte.h>
#include <SoftwareSerial.h>
#include <SensorGPS.h>
#include <TinyGPS.h>

Csuporte suporte;
TinyGPS tgps;
//SoftwareSerial ss(19, 18); // RX TX

const int NLOOPS = 20;          //Número de valores para calcular a média inicial
int contLoops = 0;             //Contador de loops
float nLatitudes[NLOOPS];      
float nLongitudes[NLOOPS];

int gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second, gps_hundredths, gps_numSat, gps_precisao;
float gps_latitude, gps_longitude, gps_altitude, gps_velocidade, gps_curse;
unsigned long gps_age;
float gps_lat_REF, gps_long_REF, gps_X, gps_Y, gps_Dist;
float gps_X_antigo, gps_Y_antigo, gps_velocidade_antigo;


SensorGPS::SensorGPS(){
  Serial3.begin(9600);
}



void SensorGPS::atualizaGPS(){

   atualizaTudo();
}





int SensorGPS::getNumSat(){
  return gps_numSat;
}

bool SensorGPS::temGPS(){
  if(abs(gps_latitude) < 360){
    return true;
  }else{
    return false;
  }
}

int SensorGPS::getPrecisao(){
  return gps_precisao;
}

float SensorGPS::getLatitude(){
  return gps_latitude;
}

float SensorGPS::getLongitude(){
  return gps_longitude;
}

int SensorGPS::getAno(){
  return gps_year; 
}

int SensorGPS::getMes(){
  return gps_month; 
}

int SensorGPS::getDia(){
  return gps_day; 
}

int SensorGPS::getHora(){
  return gps_hour; 
}

int SensorGPS::getMinuto(){
  return gps_minute; 
}

int SensorGPS::getSegundo(){
  return gps_second; 
}

int SensorGPS::getCentSegundo(){
  return gps_hundredths; 
}

int SensorGPS::getAge(){
  return gps_age;
}

float SensorGPS::getAltitude(){
  return gps_altitude; 
}

float SensorGPS::getCurse(){
  return gps_curse; 
}

float SensorGPS::getVelocidade(){
  if(gps_velocidade == -999){
    return 0;
  }
  return gps_velocidade;
}

float SensorGPS::getLatRef(){
  return gps_lat_REF;
  
}

float SensorGPS::getLongRef(){
  return gps_long_REF;
  
}

float SensorGPS::getX(){
  if(abs(gps_X) < 900){
    if(abs(gps_X - gps_X_antigo) <80){
      gps_X_antigo = gps_X;
    }else{
      gps_X = gps_X_antigo;
    }
  }else{
    gps_X = gps_X_antigo;
  }
  if(gps_X)
  return gps_X;
  
}

float SensorGPS::getY(){
  if(abs(gps_Y) < 900){
    if(abs(gps_Y - gps_Y_antigo) <80){
      gps_Y_antigo = gps_Y;
    }else{
      gps_Y = gps_Y_antigo;
    }
  }else{
    gps_Y =  gps_Y_antigo;
  }
  return gps_Y;
  
}

float SensorGPS::getDist(){
  return gps_Dist;
  
}

bool SensorGPS::setReferenciaAqui(){
  boolean sucesso = false;
  
  if((gps_latitude != -999)||(gps_longitude != -999)){            // Se não tem erro de latitude nem de longitude, muda o ponto de referência
     gps_lat_REF = gps_latitude;
     gps_long_REF = gps_longitude;
     sucesso = true;
     contLoops = 0;                                               //Seta o valor do contador de loops pra zero
  }                                                               // O que isso faz?
                                                                  //   Toda vez que um indivíduo chama essa função, momentâneamente ela vai conferir se o valor atual de latitude e longitude são válidos
  return sucesso;                                                 //   se esses valores forem válidos ela vai pegar a latitude e longitude e colocar como valor de referência.   
}                                                                 //   depois, vai ativar o contLoops, que é a variável que vai permitir alimentar o vetor de latitudes e longitudes até o valor de "NLOOPS"
                                                                  //   Assim que o vetor estiver completamente alimentado, ela vai tirar a média desses vetores e colocar este valor como referencia
                                                                  //   Dessa forma teremos inicialmente um valor de latitude e longitude "um pouco zuado", e depois de NLOOPS de atualizações
                                                                  //   teremos um valor muito mais exato da nossa localização.
                                                                  
                                                                  
float SensorGPS::calcDistXRef(float atualLongitude){
  float dist = (tgps.distance_between(0, gps_long_REF, 0, atualLongitude));
  if(atualLongitude < gps_long_REF){                                              // Caso o ponto atual esteja com longitudes mais negativas que o de referencia, multiplica a distancia por -1;
      dist = dist * (-1);
  }
  return dist;  
  
}

float SensorGPS::calcDistYRef(float atualLatitude){
  float dist = (tgps.distance_between(gps_lat_REF, 0, atualLatitude, 0));
  if(atualLatitude < gps_lat_REF){                                                // Caso o ponto atual esteja com latitudes mais negativas que o de referencia, multiplica a distancia por -1;
     dist = dist * (-1); 
  }
  return dist;
  
}

float SensorGPS::calcAbsDistRef(float atualLatitude, float atualLongitude){
  return (tgps.distance_between(gps_lat_REF, gps_long_REF, atualLatitude, atualLongitude));
  
}


void SensorGPS::atualizaTudo(){
  atualizaDadosComunicacao();
  atualizaPosicao();
  atualizaDataHora();
  atualizaAltitude();
  atualizaCurse();
  atualizaVelocidade();
  atualizaReferencia();
  
}

void SensorGPS::atualizaReferencia(){
  boolean xSucesso = false;
  boolean ySucesso = false;
  if((gps_long_REF == 0)||(gps_long_REF == -999)){     //Se a longitude de referencia estiver com erro, ou for 0 (não iniciada)
    if(gps_longitude == -999){                         //Se a longitude atual estiver com erro 
      gps_long_REF = -999;                             //Coloca erro na de referência também (não tem valor para inicializar ainda)
      gps_X = -999;                                    //Coloca erro na distancia de referência (não tem como inicializar ainda)
    }else{
      gps_long_REF = gps_longitude;                    //Inicializa referência
      gps_X = 0;                                       //Distância atual e de referência é 0
      xSucesso = true;
    }
  }else{                                               //Se a longitude de referência estiver de boa, e já foi inicializada
    gps_X = calcDistXRef(gps_longitude);               //Verifica distância entre a referência e o ponto atual, jogando na variavel gps_X
    xSucesso = true;

  }
  
  if((gps_lat_REF == 0)||(gps_lat_REF == -999)){       //Se a latitude de referencia estiver com erro, ou for 0 (não iniciada)
    if(gps_latitude == -999){                          //Se a latitude atual estiver com erro 
      gps_lat_REF = -999;                              //Coloca erro na de referência também (não tem valor para inicializar ainda)
      gps_Y = -999;                                    //Coloca erro na distancia de referência (não tem como inicializar ainda)
    }else{
      gps_lat_REF = gps_latitude;                      //Inicializa referência
      gps_Y = 0;                                       //Distância atual e de referência é 0
      ySucesso = true;
    }
  }else{                                               //Se a latitude de referência estiver de boa, e já foi inicializada
    gps_Y = calcDistYRef(gps_latitude);                //Verifica distância entre entre a latitude atual e a referência, jogando em gps_Y
    ySucesso = true;

  }
  
  if(ySucesso && xSucesso){                                       //Se tanto a longitude quanto a latidude forem válidas, calcula a distância absoluta
   gps_Dist =  calcAbsDistRef(gps_latitude, gps_longitude);
  }else{
   gps_Dist = -999; 
  }
  
   if(contLoops < NLOOPS){                                        //Em N loops de dados, vai pegar os valores da latitude e da longitude e colocar em um vetor
     if((gps_latitude != -999)&&(gps_longitude != -999)){         //Verifica validade dos dados
       nLatitudes[contLoops] = gps_latitude;
       nLongitudes[contLoops] = gps_longitude;
       contLoops++;
     }
   }else if(contLoops == NLOOPS){                                 //Quando já tiver carregado o vetor com N valores, pegamos a média deles e setamos como valor de referencia
     gps_lat_REF = suporte.mediaAnteriores(nLatitudes);
     gps_long_REF = suporte.mediaAnteriores(nLongitudes);
     contLoops++;
   }else{                                                         //Depois disso, não fazemos mais nada pois o valor de referência já está setado
       
   }
  
  
}



void SensorGPS::atualizaAltitude(){
  gps_altitude = validaDadoF(tgps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE);
}

void SensorGPS::atualizaCurse(){
  gps_curse = validaDadoF(tgps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE);
}

void SensorGPS::atualizaVelocidade(){
  gps_velocidade = validaDadoF(tgps.f_speed_mps(), TinyGPS::GPS_INVALID_F_SPEED);
}

void SensorGPS::atualizaDadosComunicacao(){
  gps_numSat = validaDadoI(tgps.satellites(), TinyGPS::GPS_INVALID_SATELLITES);
  gps_precisao = validaDadoI(tgps.hdop(), TinyGPS::GPS_INVALID_HDOP);
}


void SensorGPS::atualizaPosicao(){
  float flat, flon;
  unsigned long age;
  tgps.f_get_position(&flat, &flon, &age);
  gps_latitude = validaDadoF(flat, TinyGPS::GPS_INVALID_F_ANGLE);
  gps_longitude = validaDadoF(flon, TinyGPS::GPS_INVALID_F_ANGLE);
}

void SensorGPS::atualizaDataHora(){
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  tgps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE){
     gps_year = -999;
     gps_month = -999;
     gps_day = -999;
     gps_hour = -999;
     gps_minute = -999;
     gps_second = -999;
     gps_hundredths = -999;
     gps_age = -999;
  }else{
     gps_year = year;
     gps_month = month;
     gps_day = day;
     gps_hour = hour;
     gps_minute = minute;
     gps_second = second;
     gps_hundredths = hundredths;
     gps_age = validaDadoI(age, TinyGPS::GPS_INVALID_AGE);
  }
}


void SensorGPS::smartdelay(unsigned long ms){
  unsigned long start = millis();
  do {
    leDadoGpsEnquantoEspera();
  } while (millis() - start < ms);
}

void SensorGPS::leDadoGpsEnquantoEspera(){
  while (Serial3.available())
      tgps.encode(Serial3.read());
  
}


float SensorGPS::validaDadoF(float val, float invalid){
  float num;
  if (val == invalid){
    num = -999;
  }else{
    num = val;
  }
  return num;
}


int SensorGPS::validaDadoI(int val, float invalid){
  int num;
  if (val == invalid){
    num = -999;
  }else{
    num = val;
  }
  return num;
}
