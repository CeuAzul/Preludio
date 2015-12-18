
#include "Arduino.h"
#include "NewPingCA.h"
#include "SensorUltrassom.h"

#define CONFIRMA_NORMAL 0
#define CONFIRMA_DECOLAGEM 1
#define CONFIRMA_POUSO 2

NewPingCA sonar;      //Instancia biblioteca customisada

// TEMPO
  long ultimoTempoEncostadoNoChao = 0;
  long tempoDeVoo = 0;
  long contador = 0;
  long tempoAntesDaConfirmacao = 0;
  bool voando = false;
  int maximaDistancia;
  long distanciaTempo;
  int margem;
  int distancia;

  int millisDeConfirmacao = 250;      //Tempo de atualizações que a chave precisa estar fechada para para o contador de voo (evitar falsos negativos)
  int modoConfirmacao = CONFIRMA_NORMAL;
  bool podeResetarContador;
  long tempoAtualizacao;
  int valorPino;


SensorUltrassom::SensorUltrassom(int distanciaChao, int margemErro, int millisParaConfirmacao, int maxDist, int triggerPin, int echoPin){
//  Serial.begin(9600);
  ultimoTempoEncostadoNoChao = millis();
//  Serial.print("Trigger");
 // Serial.println(triggerPin);
//  Serial.print("echopin");
//  Serial.println(echoPin);
  pinMode(echoPin, INPUT);                                         // define o pino 8 como entrada (recebe)
  pinMode(triggerPin, OUTPUT);                                     // define o pino 12 como saida (envia)}
  sonar.start(triggerPin, echoPin, maxDist);                       // Seta os pinos da função sonar, instanciada lá em cima
  maximaDistancia = maxDist;
  distanciaTempo = distanciaChao;
  margem = margemErro;
  millisDeConfirmacao = millisParaConfirmacao;


}

void SensorUltrassom::atualizaUltrassom(){
  unsigned int uS = sonar.ping();                                  // Manda o ping e recebe o tempo em milissegundos(uS).
 //   Serial.print("ultrassom tempo: ");
//  Serial.println(uS);
  distancia = uS / US_ROUNDTRIP_CM;                                // Converte o tempo de ping para centímetros
  //tempoAgora = millis();
  
  //  if(distancia == 0){                                          //Caso a distancia for 0, ele está no infinito ou muito perto, mas já foi mudado na biblioteca customisada para aparecer 999999
  //   distancia = 999999;
  //}
  
  // Código antigo e simples
  /*
  if (distancia > (distanciaTempo+margem)){  
    contador =  (tempoAgora - ultimoTempoEncostadoNoChao);
    ultraWow = false;   
    if(contador > tempoDeVoo){
      tempoDeVoo = contador;          //Se a distância for maior que distanciaTempo+margem, significa que ta voando, então devemos colocar o tempo para contar   
    }                                	
  }
  if(distancia <= (distanciaTempo+margem) ) {                                    
    ultimoTempoEncostadoNoChao = tempoAgora;                       //Se a distância for menor que distanciaTempo+margem significa que está no chão, então resteta o tempo
    contador = 0;    
    ultraWow = true;    
  }
  */

// Código novo----------------------------------------------------------------------
  if (distancia > (distanciaTempo+margem)){  
    valorPino = 0;                           
  }
  if(distancia <= (distanciaTempo+margem) ) {                                    
    valorPino = 1;    
  }


  if(modoConfirmacao == CONFIRMA_POUSO){
    // passagem 0 -> 1
    if(valorPino == 1){
      if((millis()-tempoAntesDaConfirmacao) > millisDeConfirmacao){   // Pousou de certeza
        taNoChaoDeBoas();
        modoConfirmacao = CONFIRMA_NORMAL;
        tempoAntesDaConfirmacao = 0;
      }else{
        // Ainda não atingiu o tempo de confirmacao
      }
    }else{                      // Opa, me enganei, dado falso, não to no chão mesmo
      modoConfirmacao = CONFIRMA_NORMAL;
      tempoAntesDaConfirmacao = 0;
      taNoArDeBoas();
    }
  }else if (modoConfirmacao == CONFIRMA_DECOLAGEM){
        // passagem 1 -> 0
    if(valorPino == 0){
      if((millis()-tempoAntesDaConfirmacao) > millisDeConfirmacao){   // Ta no ar de certeza
        taNoArDeBoas();
        modoConfirmacao = CONFIRMA_NORMAL;
        tempoAntesDaConfirmacao = 0;
      }else{
        // Ainda não atingiu o tempo de confirmação
      }
    }else{                      // Opa, me enganei, não to voando ainda
      modoConfirmacao = CONFIRMA_NORMAL;
      tempoAntesDaConfirmacao = 0;
      taNoChaoDeBoas();

    }
  }else{
    if(valorPino == 1){               // Se a chave estiver fechada (no chão)
      if(voando){                       // E estava marcado como voando
        modoConfirmacao = CONFIRMA_POUSO;           // Entra na verificação pra pouso
        tempoAntesDaConfirmacao = millis();
      }else{                          // Mas não tava marcado como voando
        taNoChaoDeBoas();                           // Ta no chão de boas
      }
    }else{                          // Se a chave estiver aberta
      if(!voando){                      // Se não tava marcado como voando
        modoConfirmacao = CONFIRMA_DECOLAGEM;          // Avisa que ta voando 
        tempoAntesDaConfirmacao = millis();     
      }else{                          // Se tava marcado como voando
        taNoArDeBoas();
      }
    }
  }





}

void SensorUltrassom::calibraAquiMesmo(int nMedia){
  int distanciaMedia = 0;
  for (int i = 0; i < nMedia; i++){
    unsigned int uS = sonar.ping();                                  // Manda o ping e recebe o tempo em milissegundos(uS).
    int dist = uS / US_ROUNDTRIP_CM;                                // Converte o tempo de ping para centímetros
    if((dist > 2)&&(dist < 30)){

      distanciaMedia += dist;
    }
    delay(300);
  }
  distanciaTempo = distanciaMedia/nMedia;
}

void SensorUltrassom::calibraDistancia(int dist){
  distanciaTempo = dist;
}

void SensorUltrassom::setDistanciaMargemErro(int margemErro){
  margem = margemErro;
}

long SensorUltrassom::getTempoDeVoo(){
  return tempoDeVoo; 
}

long SensorUltrassom::getContador(){
  return contador; 
}

int SensorUltrassom::getDistancia(){
  if(distancia > maximaDistancia){
    return maximaDistancia;
  }else{
    return distancia; 
  }
}
long SensorUltrassom::getTempoDeConfirmacao(){
  return (millis()-tempoAntesDaConfirmacao);
}

bool SensorUltrassom::getWoWUltrassom(){
  return (!voando);
}

int SensorUltrassom::getDistReferencia(){
  return distanciaTempo; 
}

long SensorUltrassom::getUltimoTempoEncostadoNoChao(){
  return ultimoTempoEncostadoNoChao; 
}

bool SensorUltrassom::getWoWRaw(){
  return valorPino;
}

void SensorUltrassom::taNoChaoDeBoas(){
  voando = false;
  ultimoTempoEncostadoNoChao = millis();
}

void SensorUltrassom::taNoArDeBoas(){
  voando = true;
  contador = millis() - ultimoTempoEncostadoNoChao;      // Ta voando, pode contar tempo
  if(contador > tempoDeVoo){           // Se o tempo provisório for maior que o tempo de voo oficial
    tempoDeVoo = contador;            // Tempo de voo provisório vira o oficial
  }
}