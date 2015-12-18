#ifndef MenuDeCalibracao_h
#define MenuDeCalibracao_h

#include "Arduino.h"
#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <LCDBitmap.h>
#include <IRremote.h>
#include <SDHelper.h>
    
class MenuDeCalibracao{
  public:
    static void iniciaMenuDeCalibracao(int numDadosExibidos);
    static void atualizaMenuDados();

    static void setValorUltrassomRaw(int dist);
    static void setValorPWMRaw(int PWM1, int PWM2, int PWM3, int PWM4);
    static void setDadosParaExibir(String dadosVindo[]);

    static void menuUseEvent(MenuUseEvent used);
    static void menuChangeEvent(MenuChangeEvent changed);
    static void setTela(String msg);
    static void mostraMenuEscondido();
    static bool emModoLeitura();
    static bool lcdLigado();


  private:
    static void menuSetup();

    static void pwmConfiguraSelecionado(int selecionado);
    static void pwmConfigCancela();
    static void printaTelinhaCalibrPWM(int valor);
    static void printaTelinhaCalibrUltrassom(int valor);
    static void printaTelinhaDados(String dado);
    static void printaTelinha(char linha1[], char linha2[], bool selecaoLinha0, bool mostraEsquerda,  bool clicavel, bool mostraDireita);
    static void trataNumero(int n);
    static void startScreen();
    static void verificaControleIR();
    static void setModoApenasLeitura();
    static void ligaDesligaTela();

};





#endif
