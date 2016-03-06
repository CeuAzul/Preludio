/** @file
 *  @brief Arquivo de captura de sinal ultrassom, cálculo de distância e indicação de Wow
 *
 *  Este arquivo é usado juntamente com o módulo de ultrassom. 
 *  Pegamos a resposta de tempo de resposta e calculamos a 
 *  distância até o chão. Caso esta distâcia é maior que a 
 *  distância de calibração estipulada no início do programa, 
 *  dizemos que o avião está voando, caso contrário, está no chão.
 *  @author Leonardo Mariga (leomariga@gmail.com)
 */

#include "Arduino.h"
#include "NewPingCA.h"
#include "SensorUltrassom.h"


/**
 * Definições do estado do avião.
 */
#define CONFIRMA_NORMAL 0
#define CONFIRMA_DECOLAGEM 1
#define CONFIRMA_POUSO 2

NewPingCA sonar;    


  long ultimoTempoEncostadoNoChao = 0;                            //!< Ultimo "millis()" detectado que o avião estava no chão
  long tempoDeVoo = 0;                                            //!< Variável para alocar o maior tempo de voo
  long contador = 0;                                              //!< Variável mostra o tempo de voo atual, não leva em consideração se é maior ou menor que um possível "tempo de voo" já armazenado anteriormente
  long tempoAntesDaConfirmacao = 0;                               //!< Tempo "millis()" onde foi detectado a mudança de estado (voando/solo), mas sem ainda confirmação
  bool voando = false;                                            //!< Indicador se o avião está voando ou não
  int maximaDistancia;                                            //!< Valor limitante até o qual o ultrassom vai detectar o chão (valores grande tornam a plataforma lenta)
  long distanciaTempo;                                            //!< Distância exata que o ultrassom está do chão, caso passe deste valor, será contado como "voando"
  int margem;                                                     //!< Pequena variação de altura permitida para o ultrassom, de forma que pequenas trepidações não mudem o estado do avião sem querer
  int distancia;                                                  //!< Atual distância do avião até o solo

  int millisDeConfirmacao = 250;                                  //!< Tempo que a chave precisa estar fechada para para o contador de voo (evitar falsos negativos)
  int modoConfirmacao = CONFIRMA_NORMAL;                          //!< Atual modo do avião
  bool podeResetarContador;                                 
  long tempoAtualizacao;
  int valorPino;                                                  //!< Valor indica se o avião passou da distância de calibração ou não




/**
 * Construtor aprenas torna os parâmetros passados como global
 */
SensorUltrassom::SensorUltrassom(int distanciaChao, int margemErro, int millisParaConfirmacao, int maxDist, int triggerPin, int echoPin){
  ultimoTempoEncostadoNoChao = millis();
  pinMode(echoPin, INPUT);                                         // Define o pino 8 como entrada (recebe)
  pinMode(triggerPin, OUTPUT);                                     // Define o pino 12 como saida (envia)}
  sonar.start(triggerPin, echoPin, maxDist);                       // Deta os pinos da função sonar, instanciada lá em cima
  maximaDistancia = maxDist;
  distanciaTempo = distanciaChao;
  margem = margemErro;
  millisDeConfirmacao = millisParaConfirmacao;


}


/**
 *  Esta função possui 4 etapas principais:
 *  1 - Envia o ping do sonar e recebe o sinal de volta, 
 *  com isso é medido o tempo em microssegundos e assim, 
 *  consegue-se descobrir a distância que o ultrassom está 
 *  do chão.
 *
 *  2 - Verifica se esta distância é maior que a distância 
 *  de referência, encontrada realizando a calibração inicial.
 *  Pode-se adicionar uma margem. Esta margem existe com o 
 *  objetivo de evitar que pequenas turbulências durante a 
 *  decolagem e pouso indiquem errôneamente que o avião 
 *  está decolando ou pousando (visto que podem passar da
 *  distância de referência sem querer).
 *
 *  3 - Ter certeza que a mudança de estado aconteceu de 
 *  fato, que é feito estipulando-se um "tempo de confirmação".
 *  Para que haja a mudança de estado de "decolagem" para
 *  "pouso" ou contrário, além de passar da distância de
 *  referência, o sensor PRECISA NECESSARIAMENTE ficar mais
 *  de "millisDeConfirmacao" naquele estado. 
 *  Por exemplo, caso o avião esteja no chão e o 
 *  ultrassom indique que passou da "distancia+margem"
 *  o programa irá armazenar o valor desse instante e entrar
 *  em um "modo de confimação". Neste modo, o programa irá 
 *  verificar se as entradas das próximas X atualizações durante,
 *  por exemplo, 250 milissegudos, confirmam que ele está no ar.
 *  Se todas as atualizações confirmarem que a distância atual é
 *  maior que a de referência, o avião irá pegar o instante
 *  armazenado e setar aquele como instante de decolagem.
 *  Caso durante esses 250 milissegundos o ultrassom detecte
 *  um sinal indicando que está com uma distância menor ou igual
 *  a de calibração, todo processo é cancelado, o tempo é
 *  resetado, e o estado do avião continua como "no chão".
 *  Isso se mostrou extremamente eficiente para evitar bugs
 *  de indicações momentâneas erradas. Sem esse sistema
 *  um único sinal errado vindo do ultrassom, indicando
 *  uma distância menor que o normal, durante o voo, por 
 *  exemplo, contava como "pouso" e dava um tempo de voo
 *  errado.
 *
 *  4 - Após a confirmação da decolagem, a variável "contador"
 *  é constantemente atualizada, indicando o tempo que se passou
 *  desde o instante de decolagem. Caso o tempo de voo atual 
 *  seja maior que algum outro tempo de voo anterior, o 
 *  "tempoDeVoo" recebe o "contador". Ou seja, o tempoDeVoo
 *  real sempre será o maior tempo entre uma decolagem confirmada
 *  e um pouso confirmado. Isso é feito com o objetivo de 
 *  limitar o pouso ao primeiro instante que o avião
 *  encosta no chão, caso ele volte a subir por um curto período
 *  de tempo, a variável contador não passará seu valor para o
 *  "tempoDeVoo".
 *
 *  
 */
void SensorUltrassom::atualizaUltrassom(){
  /**
  *  Manda o ping e recebe o tempo em milissegundos(uS).
  *  Converte o tempo de ping para centímetros
  */
  unsigned int uS = sonar.ping();                             
  distancia = uS / US_ROUNDTRIP_CM;  



  /**
  *  Verifica se a distância atual é maior que a distância
  *  de referência + margem, e muda valorPino
  */                           
  if (distancia > (distanciaTempo+margem)){  
    valorPino = 0;                           
  }
  if(distancia <= (distanciaTempo+margem) ) {                                    
    valorPino = 1;    
  }



  /**
  *  Processo de confirmação de tempo
  */ 
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
    }else{                                                            // Opa, me enganei, dado falso, não to no chão mesmo
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
    }else{                                                            // Opa, me enganei, não to voando ainda
      modoConfirmacao = CONFIRMA_NORMAL;
      tempoAntesDaConfirmacao = 0;
      taNoChaoDeBoas();

    }
  }else{
    if(valorPino == 1){                                               // Se a chave estiver fechada (no chão)
      if(voando){                                                     // E estava marcado como voando
        modoConfirmacao = CONFIRMA_POUSO;                             // Entra na verificação pra pouso
        tempoAntesDaConfirmacao = millis();
      }else{                                                          // Mas não tava marcado como voando
        taNoChaoDeBoas();                                             // Ta no chão de boas
      }
    }else{                                                            // Se a chave estiver aberta
      if(!voando){                                                    // Se não tava marcado como voando
        modoConfirmacao = CONFIRMA_DECOLAGEM;                         // Avisa que ta voando 
        tempoAntesDaConfirmacao = millis();     
      }else{                                                          // Se tava marcado como voando
        taNoArDeBoas();
      }
    }
  }
}


 /**
  *  Função irá calibrar o ultrassom no local que ela for chamada
  *  medindo diversas vezes a distância até o chão e fazendo a
  *  média. Ao final da calibração, a variável "distanciaTempo"
  *  terá recebido a distância do ultrassom até o chão.
  *
  */ 
void SensorUltrassom::calibraAquiMesmo(int nMedia){
  int distanciaMedia = 0;
  for (int i = 0; i < nMedia; i++){
    unsigned int uS = sonar.ping();                                  // Manda o ping e recebe o tempo em milissegundos(uS).
    int dist = uS / US_ROUNDTRIP_CM;                                 // Converte o tempo de ping para centímetros

      /**
      *  Verifica se a distância não ta zoada
      */ 
    if((dist > 2)&&(dist < 30)){
      distanciaMedia += dist;
    }

    /**
    *  Coloca o delay maroto pra não bugar
    */ 
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

/**
*  Função complementar da "atualizaUltrassom"
*/ 
void SensorUltrassom::taNoChaoDeBoas(){
  voando = false;
  ultimoTempoEncostadoNoChao = millis();
}

/**
*  Função complementar da "atualizaUltrassom"
*/ 
void SensorUltrassom::taNoArDeBoas(){
  voando = true;
  contador = millis() - ultimoTempoEncostadoNoChao;      // Ta voando, pode contar tempo
  if(contador > tempoDeVoo){                             // Se o tempo provisório for maior que o tempo de voo oficial
    tempoDeVoo = contador;                               // Tempo de voo provisório vira o oficial
  }
}