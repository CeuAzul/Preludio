#include <Csuporte.h>
#include <SoftwareSerial.h>

#include <TinyGPS.h>

Csuporte suporte;
TinyGPS gps;
SoftwareSerial ss(4, 3);

const int NLOOPS = 5;          //Número de valores para calcular a média inicial
int contLoops = 0;             //Contador de loops
float nLatitudes[NLOOPS];      
float nLongitudes[NLOOPS];

int gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second, gps_hundredths, gps_numSat, gps_precisao;
float gps_latitude, gps_longitude, gps_altitude, gps_velocidade, gps_curse;
unsigned long gps_age;
float gps_lat_REF, gps_long_REF, gps_X, gps_Y, gps_Dist;



static int getNumSat();           //Retorna número de satélites em contato
static int getPrecisao();         //Retorna medida de precisão de dados
static float getLatitude();       //Retorna latitude
static float getLongitude();      //Retorna longitude
static int getAge();              //Retorna a idade do ultimo dado
static int getAno();              //Retorna ano
static int getMes();              //Retorna mês
static int getDia();              //Retorna dia
static int getHora();             //Retorna hora
static int getMin();              //Retorna minutos
static int getSegundo();          //Retorna segundo
static int getCentSegundo();      //Retorna centésimo de segundo (não funciona por algum motivo)
static float getAltitude();       //Retorna altura
static float getVelocidade();     //Retorna velocidade 
static float getCurse();          //Retorna curse of ground
static float getX();              //Retorna a variavel de distância X (longitudinal) até a referência
static float getY();              //Retorna a variavel de distância Y (latitudinal) até a referência
static float getDist();           //Retorna a variavel da distância absoluta até a referência
static float getLatRef();         //Pega a latitude de referência atual
static float getLongRef();        //Pega longitude atual de referência


static boolean setReferenciaAqui();                                     //Seta a posição atual como referência para se medir distância e retorna se obteve sucesso em fazer isso
static float calcDistXRef(float atualLongitude);                        //Pega a distância X em metros, entre a longitude do parâmetro e a longitude de referência
static float calcDistYRef(float atualLatitude);                         //Pega a distancia Y em metros, entre a latitude do parâmetro e a latitude de referência
static float calcAbsDistRef(float atualLatitude, float atualLongitude); //Pega a distância absoluta em metros entre o ponto do parâmetro e o ponto de referência


static void atualizaTudo();                //Atualiza todos parâmetros      
static void atualizaReferencia();          //Atualiza dados de referência do satélite
static void atualizaAltitude();            //Atualiza altitude
static void atualizaCurse();               //Atualiza curse
static void atualizaVelocidade();          //Atualza velocidade
static void atualizaDadosComunicacao();    //Atualiza dados de comunicação (numSat e posição)
static void atualizaPosicao();             //Atualiza posição (latitude e longitude)
static void atualizaDataHora();            //Atualiza data e hora

static float validaDadoF(float val, float invalid);         //Valida dados ou retorna erro
static int validaDadoI(int val, float invalid);             //Valida dados ou retorna erro (para inteiros)

static void leDadoGpsEnquantoEspera();     // Enquanto está esperando, aproveita para ler dados da serial


void setup(){
  Serial.begin(9600);
  
// Texto Inicial

  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude    Date     Time    Date   Alt    Course Speed Cardinal   ");
  Serial.println("          (deg)     (deg)                          Age   (m)           (kmh)           ");
  Serial.println("--------------------------------------------------------------------------------------");

  ss.begin(9600);
}

void loop(){

   atualizaTudo();

  

   Serial.print(getNumSat());    
   Serial.print("  ");
   Serial.print(getPrecisao());     
   Serial.print("  ");
   Serial.print(getLatitude(), 11);        
   Serial.print("  ");
   Serial.print(getLongitude(), 11); 
   Serial.print("  ");
   Serial.print(getAge());    
   Serial.print("  ");
   Serial.print(getAno());    
   Serial.print("  ");
   Serial.print(getMes());     
   Serial.print("  ");
   Serial.print(getDia());        
   Serial.print("  ");
   Serial.print(getHora()); 
   Serial.print("  ");
   Serial.print(getMinuto());   
   Serial.print("  ");
   Serial.print(getSegundo());     
   Serial.print("  ");
   Serial.print(getCentSegundo());        
   Serial.print("  ");
   Serial.print(getAltitude(), 4);   
   Serial.print("  ");
   Serial.print(getCurse(), 4);     
   Serial.print("  ");
   Serial.print(getVelocidade(), 4);        
   Serial.print("  ");
   Serial.print(getX(), 4);   
   Serial.print("  ");
   Serial.print(getY(), 4);     
   Serial.print("  ");
   Serial.print(getDist(), 4);        
   Serial.print("  ");
   Serial.print(getLatRef(), 11);     
   Serial.print("  ");
   Serial.print(getLongRef(), 11);        

   

   



  Serial.println();
  
  smartdelay(1000);
}





static int getNumSat(){
  return gps_numSat;
}

static int getPrecisao(){
  return gps_precisao;
}

static float getLatitude(){
  return gps_latitude;
}

static float getLongitude(){
  return gps_longitude;
}

static int getAno(){
  return gps_year; 
}

static int getMes(){
  return gps_month; 
}

static int getDia(){
  return gps_day; 
}

static int getHora(){
  return gps_hour; 
}

static int getMinuto(){
  return gps_minute; 
}

static int getSegundo(){
  return gps_second; 
}

static int getCentSegundo(){
  return gps_hundredths; 
}

static int getAge(){
  return gps_age;
}

static float getAltitude(){
  return gps_altitude; 
}

static float getCurse(){
  return gps_curse; 
}

static float getVelocidade(){
  return gps_velocidade;
}

static float getLatRef(){
  return gps_lat_REF;
  
}

static float getLongRef(){
  return gps_long_REF;
  
}

static float getX(){
  return gps_X;
  
}

static float getY(){
  return gps_Y;
  
}

static float getDist(){
  return gps_Dist;
  
}

static boolean setReferenciaAqui(){
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
                                                                  
                                                                  
static float calcDistXRef(float atualLongitude){
  float dist = (gps.distance_between(0, gps_long_REF, 0, atualLongitude));
  if(atualLongitude < gps_long_REF){                                              // Caso o ponto atual esteja com longitudes mais negativas que o de referencia, multiplica a distancia por -1;
      dist = dist * (-1);
  }
  return dist;  
  
}

static float calcDistYRef(float atualLatitude){
  float dist = (gps.distance_between(gps_lat_REF, 0, atualLatitude, 0));
  if(atualLatitude < gps_lat_REF){                                                // Caso o ponto atual esteja com latitudes mais negativas que o de referencia, multiplica a distancia por -1;
     dist = dist * (-1); 
  }
  return dist;
  
}

static float calcAbsDistRef(float atualLatitude, float atualLongitude){
  return (gps.distance_between(gps_lat_REF, gps_long_REF, atualLatitude, atualLongitude));
  
}


static void atualizaTudo(){
  atualizaDadosComunicacao();
  atualizaPosicao();
  atualizaDataHora();
  atualizaAltitude();
  atualizaCurse();
  atualizaVelocidade();
  atualizaReferencia();
  
}

static void atualizaReferencia(){
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



static void atualizaAltitude(){
  gps_altitude = validaDadoF(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE);
  smartdelay(0);
}

static void atualizaCurse(){
  gps_curse = validaDadoF(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE);
  smartdelay(0);
}

static void atualizaVelocidade(){
  gps_velocidade = validaDadoF(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED);
  smartdelay(0);
}

static void atualizaDadosComunicacao(){
  gps_numSat = validaDadoI(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES);
  smartdelay(0);
  gps_precisao = validaDadoI(gps.hdop(), TinyGPS::GPS_INVALID_HDOP);
  smartdelay(0);
}


static void atualizaPosicao(){
  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);
  gps_latitude = validaDadoF(flat, TinyGPS::GPS_INVALID_F_ANGLE);
  smartdelay(0);
  gps_longitude = validaDadoF(flon, TinyGPS::GPS_INVALID_F_ANGLE);
  smartdelay(0);
}

static void atualizaDataHora(){
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
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
  smartdelay(0);
}


static void smartdelay(unsigned long ms){
  unsigned long start = millis();
  do {
    leDadoGpsEnquantoEspera();
  } while (millis() - start < ms);
}

static void leDadoGpsEnquantoEspera(){
  while (ss.available())
      gps.encode(ss.read());
  
}


static float validaDadoF(float val, float invalid){
  float num;
  if (val == invalid){
    num = -999;
  }else{
    num = val;
  }
  return num;
}


static int validaDadoI(int val, float invalid){
  int num;
  if (val == invalid){
    num = -999;
  }else{
    num = val;
  }
  return num;
}
