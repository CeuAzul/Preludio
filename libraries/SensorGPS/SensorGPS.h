/*
  GPS
*/

#ifndef SensorGPS_h
#define SensorGPS_h

#include "Arduino.h"
#include "TinyGPS.h"

class SensorGPS{
  public:
    SensorGPS();

     int getNumSat();           //Retorna número de satélites em contato
     int getPrecisao();         //Retorna medida de precisão de dados
     float getLatitude();       //Retorna latitude
     float getLongitude();      //Retorna longitude
     int getAge();              //Retorna a idade do ultimo dado
     int getAno();              //Retorna ano
     int getMes();              //Retorna mês
     int getDia();              //Retorna dia
     int getHora();             //Retorna hora
     int getMinuto();           //Retorna minutos
     int getSegundo();          //Retorna segundo
     int getCentSegundo();      //Retorna centésimo de segundo (não funciona por algum motivo)
     float getAltitude();       //Retorna altura
     float getVelocidade();     //Retorna velocidade 
     float getCurse();          //Retorna curse of ground
     float getX();              //Retorna a variavel de distância X (longitudinal) até a referência
     float getY();              //Retorna a variavel de distância Y (latitudinal) até a referência
     float getDist();           //Retorna a variavel da distância absoluta até a referência
     float getLatRef();         //Pega a latitude de referência atual
     float getLongRef();        //Pega longitude atual de referência
     bool temGPS();             //Verifica se pegou satélite ou não
    
     bool setReferenciaAqui();                                     //Seta a posição atual como referência para se medir distância e retorna se obteve sucesso em fazer isso
     float calcDistXRef(float atualLongitude);                        //Pega a distância X em metros, entre a longitude do parâmetro e a longitude de referência
     float calcDistYRef(float atualLatitude);                         //Pega a distancia Y em metros, entre a latitude do parâmetro e a latitude de referência
     float calcAbsDistRef(float atualLatitude, float atualLongitude); //Pega a distância absoluta em metros entre o ponto do parâmetro e o ponto de referência
    
     void atualizaGPS();                        //Loop do gps
     void atualizaTudo();                //Atualiza todos parâmetros      
     void atualizaReferencia();          //Atualiza dados de referência do satélite
     void atualizaAltitude();            //Atualiza altitude
     void atualizaCurse();               //Atualiza curse
     void atualizaVelocidade();          //Atualza velocidade
     void atualizaDadosComunicacao();    //Atualiza dados de comunicação (numSat e posição)
     void atualizaPosicao();             //Atualiza posição (latitude e longitude)
     void atualizaDataHora();            //Atualiza data e hora
       
     void leDadoGpsEnquantoEspera();     // Enquanto está esperando, aproveita para ler dados da serial
     void smartdelay(unsigned long ms);
  private: 
     float validaDadoF(float val, float invalid);         //Valida dados ou retorna erro
     int validaDadoI(int val, float invalid);             //Valida dados ou retorna erro (para inteiros)



};


#endif
