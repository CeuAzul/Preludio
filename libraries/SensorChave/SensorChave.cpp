#include "Arduino.h"
#include "SensorChave.h"

#define CONFIRMA_NORMAL 0
#define CONFIRMA_DECOLAGEM 1
#define CONFIRMA_POUSO 2

namespace variaveisChave{
	int pin = 49;
	long ultimoTempoEncostadoNoChao = 0;
	long tempoDeVoo = 0;
	long contador = 0;
	long tempoAntesDaConfirmacao = 0;
	bool voando = false;

	int millisDeConfirmacao = 2;			//Tempo de atualizações que a chave precisa estar fechada para para o contador de voo (evitar falsos negativos)
	int modoConfirmacao = CONFIRMA_NORMAL;
	bool podeResetarContador;
	long tempoAtualizacao;
	int valorPino;
}

using namespace variaveisChave;

SensorChave::SensorChave(int millisParaConfirmacao, int pino){
  pinMode(pin, INPUT);      // sets the digital pin 7 as input
  millisDeConfirmacao = millisParaConfirmacao;
  pin = pino;
}


void SensorChave::atualizaChave(){
	valorPino = digitalRead(pin);


	if(modoConfirmacao == CONFIRMA_POUSO){
		// passagem 0 -> 1
		if(valorPino == 1){
			if((millis()-tempoAntesDaConfirmacao) > millisDeConfirmacao){		// Pousou de certeza
				taNoChaoDeBoas();
				modoConfirmacao = CONFIRMA_NORMAL;
				tempoAntesDaConfirmacao = 0;
			}else{
				// Ainda não atingiu o tempo de confirmacao
			}
		}else{											// Opa, me enganei, dado falso, não to no chão mesmo
			modoConfirmacao = CONFIRMA_NORMAL;
			tempoAntesDaConfirmacao = 0;
			taNoArDeBoas();
		}
	}else if (modoConfirmacao == CONFIRMA_DECOLAGEM){
				// passagem 1 -> 0
		if(valorPino == 0){
			if((millis()-tempoAntesDaConfirmacao) > millisDeConfirmacao){		// Ta no ar de certeza
				taNoArDeBoas();
				modoConfirmacao = CONFIRMA_NORMAL;
				tempoAntesDaConfirmacao = 0;
			}else{
				// Ainda não atingiu o tempo de confirmação
			}
		}else{											// Opa, me enganei, não to voando ainda
			modoConfirmacao = CONFIRMA_NORMAL;
			tempoAntesDaConfirmacao = 0;
			taNoChaoDeBoas();

		}
	}else{
		if(valorPino == 1){								// Se a chave estiver fechada (no chão)
			if(voando){												// E estava marcado como voando
				modoConfirmacao = CONFIRMA_POUSO;						// Entra na verificação pra pouso
				tempoAntesDaConfirmacao = millis();
			}else{													// Mas não tava marcado como voando
				taNoChaoDeBoas();														// Ta no chão de boas
			}
		}else{													// Se a chave estiver aberta
			if(!voando){											// Se não tava marcado como voando
				modoConfirmacao = CONFIRMA_DECOLAGEM;					 // Avisa que ta voando	
				tempoAntesDaConfirmacao = millis();			
			}else{													// Se tava marcado como voando
				taNoArDeBoas();
			}
		}
	}
}


int SensorChave::getValorDaChave(){
	return valorPino;
}

long SensorChave::getContadorChave(){
	return contador;
}

long SensorChave::getUltimoTempoEncostadoNoChaoChave(){
	return ultimoTempoEncostadoNoChao;
}

long SensorChave::getTempoDeConfirmacao(){
	return (millis()-tempoAntesDaConfirmacao);
}

long SensorChave::getTempoDeVooChave(){
	return tempoDeVoo;
}

bool SensorChave::getWOW(){
	return (!voando);
}


void SensorChave::taNoChaoDeBoas(){
	voando = false;
	ultimoTempoEncostadoNoChao = millis();
}

void SensorChave::taNoArDeBoas(){
	voando = true;
	contador = millis() - ultimoTempoEncostadoNoChao;			 // Ta voando, pode contar tempo
	if(contador > tempoDeVoo){					 // Se o tempo provisório for maior que o tempo de voo oficial
		tempoDeVoo = contador;					 	// Tempo de voo provisório vira o oficial
	}
}