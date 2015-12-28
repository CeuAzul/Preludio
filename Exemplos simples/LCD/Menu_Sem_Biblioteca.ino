/**
 * HelloMenu
 * by BREVIG http://alexanderbrevig.com
 * 
 * This is the structure of the modelled menu
 * 
 * Menu Telemetria
 * Iniciar Log >
 *   Modo exibicao
 *   Modo sem LCD 
 * Config >
 *   Calibrar Ultrassom 
 *   Calibrar PWM 1
 *   Calibrar PWM 2
 *   Calibrar PWM 3
 *   Calibrar PWM 4
 */

#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <LCDBitmap.h>
#include <IRremote.h>
#include <SDHelper.h>

SDHelper sdConfig;

const long  IR_0    = 0x20DF08F7;
const long  IR_1    = 0x20DF8877;
const long  IR_2    = 0x20DF48B7;
const long  IR_3    = 0x20DFC837;
const long  IR_4    = 0x20DF28D7;
const long  IR_5    = 0x20DFA857;
const long  IR_6    = 0x20DF6897;
const long  IR_7    = 0x20DFE817;
const long  IR_8    = 0x20DF18E7;
const long  IR_9    = 0x20DF9867;
const long  IR_OK   = 0x20DF22DD;
const long  IR_CIMA   = 0x20DF02FD;
const long  IR_BAIXO  = 0x20DF827D;
const long  IR_ESQUERDA = 0x20DFE01F;
const long  IR_DIREITA  = 0x20DF609F;
const long  IR_LIGA   = 0x20DF10EF;
const long  IR_MENOS  = 0x20DF32CD;
const long  IR_PLAY   = 0x20DF0DF2;
const long  IR_PAUSE  = 0x20DF5DA2;
const long  IR_STOP   = 0x20DF8D72;
const long  IR_VOLTA  = 0x20DF14EB;


char MENU_INICIALOG[] = "Iniciar Log";
char MENU_INICIALOG_MODOEXIB[] = "Modo Exibicao";
char MENU_INICIALOG_MODOSEMLCD[] = "Modo sem LCD";
char MENU_CONFIG[] = "Config";
char MENU_CONFIG_CALIBRAR[] = "Calibracao";
char MENU_CONFIG_CALIBRAR_ULTRASSOM[] = "Calib. Ultra.";
char MENU_CONFIG_CALIBRAR_PWM1[] = "Calib. Prof.";
char MENU_CONFIG_CALIBRAR_PWM2[] = "Calib. Leme";
char MENU_CONFIG_CALIBRAR_PWM3[] = "Calib. Ailer.";
char MENU_CONFIG_CALIBRAR_PWM4[] = "Calib. Motor";
char MENU_DADOS[] = "Dados";
char MENU_DADOS_VER_DADOS[] = "Ver Dados";

int dadoNum;
int numTotalDados = 25;
bool taNaTelaDeDados = false;
bool taNaTelaCalibUltra = false;
bool telaDeDadosCarregada = false;
bool taNaTelaCalibPWM = false;
bool taNoPWMZero = false;
bool taNoPWMMax = false;
bool taNoModoInput = false;
bool calibracaoPronta = false;
String ultimoDadoExibido;
int pwmExibido = -1;
int calibNum1=-1;
int calibNum2=-1;
int calibPWMref0 = 1400;
int calibPWMrefMax = 1800;

//Controle IR

long armazenavalor;
int RECV_PIN = 7;

IRrecv irrecv(RECV_PIN);

decode_results results;
//-------------------

LiquidCrystal lcd(22, 24, 26, 28, 30, 32, 34); //Configura os pinos do Arduino para se comunicar com o LCD

LCDBitmap bitmap(&lcd, 11, 0);
//this controls the menu backend and the event generation
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
  //beneath is list of menu items needed to build the menu
  MenuItem iniciarLog   = MenuItem(menu, MENU_INICIALOG, 1);
    MenuItem modoExib      = MenuItem(menu, MENU_INICIALOG_MODOEXIB, 2);
    MenuItem modoSemLCD    = MenuItem(menu, MENU_INICIALOG_MODOSEMLCD, 2);
  MenuItem configMenu    = MenuItem(menu, MENU_CONFIG, 1);
    MenuItem calibUltrassom = MenuItem(menu, MENU_CONFIG_CALIBRAR_ULTRASSOM, 2);
    MenuItem calibPWM1 = MenuItem(menu, MENU_CONFIG_CALIBRAR_PWM1, 2);
    MenuItem calibPWM2 = MenuItem(menu, MENU_CONFIG_CALIBRAR_PWM2, 2);
    MenuItem calibPWM3 = MenuItem(menu, MENU_CONFIG_CALIBRAR_PWM3, 2);
    MenuItem calibPWM4 = MenuItem(menu, MENU_CONFIG_CALIBRAR_PWM4, 2);
MenuItem dados    = MenuItem(menu, MENU_DADOS, 1);
    MenuItem verDados    = MenuItem(menu, MENU_DADOS_VER_DADOS, 2);

//this function builds the menu and connects the correct items together
void menuSetup(){
  Serial.println("Setting up menu...");
  //add the file menu to the menu root
  menu.getRoot().add(iniciarLog); 
    //setup the settings menu item

    iniciarLog.addAfter(configMenu); 
    configMenu.addAfter(dados);

    
    modoSemLCD.addLeft(iniciarLog);
    modoExib.addLeft(iniciarLog); //loop back if left on settings
    iniciarLog.addRight(modoExib);
    modoExib.addAfter(modoSemLCD);     //chain settings to  pin on right

    
        calibUltrassom.addAfter(calibPWM1);
        calibPWM1.addAfter(calibPWM2);
        calibPWM2.addAfter(calibPWM3);
        calibPWM3.addAfter(calibPWM4);    //loop back to d100
        //we want left to always be bak to delay
        calibPWM1.addLeft(configMenu);
        calibPWM2.addLeft(configMenu);
        calibPWM3.addLeft(configMenu);
        calibPWM4.addLeft(configMenu);
        calibUltrassom.addLeft(configMenu);

    dados.addRight(verDados);
}

void menuProgramTeste(){


}

/*
  This is an important function
  Here all use events are handled
  
  This is where you define a behaviour for a menu item
*/
void menuUseEvent(MenuUseEvent used){
  Serial.print("Menu use ");
  Serial.println(used.item.getName());
  
  if(used.item.getName()==MENU_DADOS_VER_DADOS){
      if(taNaTelaDeDados == false){
        dadoNum = 0;
      }else{
        if(dadoNum >= numTotalDados-1){
          dadoNum = 0;
        }else{
          dadoNum++;
        }
      }
      taNaTelaDeDados = true;
  }

  if(used.item.getName()==MENU_CONFIG_CALIBRAR_ULTRASSOM){
    if(!taNaTelaCalibUltra){
      taNaTelaCalibUltra = true;
    }else{
      if(calibracaoPronta){
        sdConfig.calibraUltrassomNoSD((calibNum1*10+calibNum2));
        Serial.println((calibNum1*10+calibNum2));
        calibNum1 = -1;
        calibNum2 = -1;
        taNoModoInput = false;
        calibracaoPronta = false;
        menu.moveLeft();
      }
    }
  }

  if(used.item.getName()==MENU_CONFIG_CALIBRAR_PWM1){
    pwmConfiguraSelecionado(1);
  }

  if(used.item.getName()==MENU_CONFIG_CALIBRAR_PWM2){
    pwmConfiguraSelecionado(2);
  }

  if(used.item.getName()==MENU_CONFIG_CALIBRAR_PWM3){
    pwmConfiguraSelecionado(3);
  }

  if(used.item.getName()==MENU_CONFIG_CALIBRAR_PWM4){
    pwmConfiguraSelecionado(4);
  }

}

/*
  This is an important function
  Here we get a notification whenever the user changes the menu
  That is, when the menu is navigated
*/
void menuChangeEvent(MenuChangeEvent changed){

  Serial.println(changed.to.getName());
  if(changed.from.getName() == MENU_DADOS_VER_DADOS){
      taNaTelaDeDados = false;
      telaDeDadosCarregada = false;

  }
  if(changed.from.getName() == MENU_CONFIG_CALIBRAR_ULTRASSOM){
      taNaTelaCalibUltra = false;
      taNoModoInput = false;
      calibNum1 = -1;
      calibNum2 = -1;
      lcd.noBlink();

  }
  if(changed.from.getName() == MENU_CONFIG_CALIBRAR_PWM1){
      pwmConfigCancela();

  }
  if(changed.from.getName() == MENU_CONFIG_CALIBRAR_PWM2){
      pwmConfigCancela();

  }
  if(changed.from.getName() == MENU_CONFIG_CALIBRAR_PWM3){
      pwmConfigCancela();

  }
  if(changed.from.getName() == MENU_CONFIG_CALIBRAR_PWM4){
      pwmConfigCancela();

  }
  if(changed.to.getName() == MENU_INICIALOG){
      printaTelinha(MENU_INICIALOG, MENU_CONFIG, true, false, false, true);

  }
  if(changed.to.getName() == MENU_CONFIG){
      printaTelinha(MENU_CONFIG, MENU_DADOS, true, false, false, true);

  }
  if(changed.to.getName() == MENU_DADOS){
      printaTelinha(MENU_CONFIG, MENU_DADOS, false, false, false, true);

  }
  if(changed.to.getName() == MENU_INICIALOG_MODOEXIB){
      printaTelinha(MENU_INICIALOG_MODOEXIB, MENU_INICIALOG_MODOSEMLCD, true, true, true, false);

  }  
  if(changed.to.getName() == MENU_INICIALOG_MODOSEMLCD){
      printaTelinha(MENU_INICIALOG_MODOEXIB, MENU_INICIALOG_MODOSEMLCD, false, true, true, false);

  } 
  if(changed.to.getName() == MENU_CONFIG_CALIBRAR_ULTRASSOM){
      printaTelinha(MENU_CONFIG_CALIBRAR_ULTRASSOM, MENU_CONFIG_CALIBRAR_PWM1, true, true, true, false);

  } 
  if(changed.to.getName() == MENU_CONFIG_CALIBRAR_PWM1){
      printaTelinha(MENU_CONFIG_CALIBRAR_ULTRASSOM, MENU_CONFIG_CALIBRAR_PWM1, false, true, true, false);

  }
  if(changed.to.getName() == MENU_CONFIG_CALIBRAR_PWM2){
      printaTelinha(MENU_CONFIG_CALIBRAR_PWM1, MENU_CONFIG_CALIBRAR_PWM2, false, true, true, false);

  }    
  if(changed.to.getName() == MENU_CONFIG_CALIBRAR_PWM3){
      printaTelinha(MENU_CONFIG_CALIBRAR_PWM2, MENU_CONFIG_CALIBRAR_PWM3, false, true, true, false);

  }
  if(changed.to.getName() == MENU_CONFIG_CALIBRAR_PWM4){
      printaTelinha(MENU_CONFIG_CALIBRAR_PWM3, MENU_CONFIG_CALIBRAR_PWM4, false, true, true, false);

  }  
  if(changed.to.getName() == MENU_DADOS_VER_DADOS){
      printaTelinha(MENU_DADOS_VER_DADOS, "", true, true, true, false);

  }  
}

void pwmConfiguraSelecionado(int selecionado){
  if(!taNaTelaCalibPWM){
      bitmap.clear();
      lcd.clear();
      lcd.print("Deixe em 0 grau");
      lcd.setCursor(0,1);
      lcd.print("e aparte ok");
      delay(3000);
      pwmExibido = selecionado;
      taNaTelaCalibPWM = true;
      taNoPWMZero = true;
      Serial.print("entrou no 0");
    }else{
      if(taNoPWMZero){
        lcd.clear();
        bitmap.clear();
        lcd.setCursor(0,0);
        lcd.print("Coloque uma");
        lcd.setCursor(0,1);
        lcd.print("  deflexao e ok");
        delay(3000);
        taNoPWMZero = false;
        taNoPWMMax = true;
        Serial.print("entrou no max");
      }else{
        if(taNoPWMMax){
          Serial.println("Ta no PWMmax"); 
          if(calibracaoPronta){
            sdConfig.calibraPWMNoSD(selecionado, calibPWMref0, calibPWMrefMax, (calibNum1*10 + calibNum2));
            Serial.print("PWM: ");  
            Serial.print(selecionado);          
            Serial.print("   Zero: ");
            Serial.print(calibPWMref0);             
            Serial.print("    Max PWM:");
            Serial.print(calibPWMrefMax);      
            Serial.print("    Max GRAUS:");
            Serial.println((calibNum1*10 + calibNum2));   
            pwmConfigCancela();
            menu.moveLeft();

          }

        }
      }

    }
}

void pwmConfigCancela(){
      calibNum1 = -1;
      calibNum2 = -1;
      taNoModoInput = false;
      taNaTelaCalibPWM = false;
      calibracaoPronta = false;
      taNoPWMMax = false;
      taNoPWMZero = false;
      calibPWMref0 = 1400;
      calibPWMrefMax = 1800;
      pwmExibido = -1;
      lcd.noBlink();
}


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  bitmap.begin();
  startScreen();
  menuSetup();
  numTotalDados = 5;
  bitmap.clear();
  menu.moveDown();
  irrecv.enableIRIn(); // Start the receiver
  //fire Delay : menu.use('D');
  //bail back to Options by moving up one logical level after a D : menu.moveRelativeLevels(-1);
}

void loop(){
  if (Serial.available()) {
    byte read = Serial.read();
    switch (read) {
      case 'w': menu.moveUp(); break;
      case 's': menu.moveDown(); break;
      case 'd': menu.moveRight(); break;
      case 'a': menu.moveLeft(); break;
      case 'e': menu.use(); break;
      case '1': trataNumero(1); break;
      case '2': trataNumero(2); break;
      case '3': trataNumero(3); break;
      case '4': trataNumero(4); break;
      case '5': trataNumero(5); break;
      case '6': trataNumero(6); break;
      case '7': trataNumero(7); break;
      case '8': trataNumero(8); break;
      case '9': trataNumero(9); break;
      case '0': trataNumero(0); break;
    }
  }
  String oi[5] = {"ESSA", "MERDA", "NAO", "FUNCIONA", "QUERO DORMIR"};
  setDadosParaExibir(oi);
  setValorUltrassomRaw(7);
  setValorPWMRaw(1300, 1500, 1400, 1500);
  verificaControleIR();
  delay(50);


}

void setValorUltrassomRaw(int dist){
    if(taNaTelaCalibUltra){
      printaTelinhaCalibrUltrassom(dist);
    }
}

void setValorPWMRaw(int PWM1, int PWM2, int PWM3, int PWM4){
    if(taNaTelaCalibPWM){
      switch (pwmExibido) {
          case 1:
            printaTelinhaCalibrPWM(PWM1);
            break;
          case 2:
            printaTelinhaCalibrPWM(PWM2);
            break;
          case 3:
            printaTelinhaCalibrPWM(PWM3);
            break;
          case 4:
            printaTelinhaCalibrPWM(PWM4);
            break;
          default:
            break;
      }

    }
}


void setDadosParaExibir(String dadosVindo[]){
  if(taNaTelaDeDados){
    printaTelinhaDados(dadosVindo[dadoNum]);
  }
}


void printaTelinhaCalibrPWM(int valor){
  lcd.clear();
  bitmap.clear();
  if(taNoPWMZero){
    bitmap.move(0,0);
    bitmap.line(0, 7, 9, 7, true, UPDATE);
    lcd.setCursor(2, 0);
    lcd.print("Deixe 0 graus");
    lcd.setCursor(2, 1);
    lcd.print("Raw:");
    lcd.print(valor);
    lcd.setCursor(13,1);
    lcd.print((char)255);
    lcd.print("Ok");
    calibPWMref0 = valor;
  }
  if(taNoPWMMax){
      bitmap.move(0,0);
      int valdiv = 7+((valor - calibPWMref0)/(-80));
      if(valdiv > 15){
        bitmap.line(0, 7, 9, 15, true, UPDATE);        
      }else{
        if(valdiv < 0){
          bitmap.line(0, 7, 9, 0, true, UPDATE);
        }else{
          bitmap.line(0, 7, 9, valdiv, true, UPDATE);
        }
      }
      lcd.setCursor(2, 1);
      lcd.print("Raw:");
      lcd.print(valor);
      lcd.setCursor(2, 0);
      lcd.print("Graus:");
      lcd.blink();
      taNoModoInput = true;
      if(calibNum1 != -1){
        lcd.print(calibNum1);
      }
      if(calibNum2 != -1){
        lcd.print(calibNum2);
        lcd.setCursor(13,1);
        lcd.noBlink();
        lcd.print((char)255);
        lcd.print("Ok");
        calibPWMrefMax = valor;
        calibracaoPronta = true;
        Serial.println("Calibração pronta, esperando ok");
       }
  }
}

void printaTelinhaCalibrUltrassom(int valor){
  lcd.clear();
  bitmap.move(0,0);
  if((valor < 16)&&(valor >= 0)){
    bitmap.rectFill(0, 15, 4, (15-valor), true, UPDATE);
  }else{
   // bitmap.rectFill(0, 15, 4, 0, true, UPDATE);
  }

  int calib = calibNum1*10+calibNum2;
//  Serial.println(calib);
  if((calib < 16)&&(calib >= 0)){
    Serial.println("CAIU NO CALIB");
    bitmap.rectFill(5, (15-calib), 9, 15, true, UPDATE);
  }else{
        Serial.println("CAIU NO ELSE");
    //bitmap.rectFill(5, 15, 9, 0, true, UPDATE);
  }
  lcd.setCursor(2,1);
  lcd.print("Raw: ");
  lcd.print(valor);
  lcd.print(" cm");
  lcd.setCursor(2,0);
  lcd.print("Digite:");
  lcd.blink();
  taNoModoInput = true;
  if(calibNum1 != -1){
    lcd.print(calibNum1);
  }
  if(calibNum2 != -1){
    lcd.print(calibNum2);
    lcd.setCursor(13,1);
    lcd.noBlink();
    lcd.print((char)255);
    lcd.print("Ok");
    calibracaoPronta = true;
  }
}

void printaTelinhaDados(String dado){
    char buff[50];
    dado.toCharArray(buff, 50);
    if(!telaDeDadosCarregada){
      lcd.clear();
      lcd.print(buff);
      lcd.setCursor(0,1);
      lcd.print("<");
      lcd.setCursor(7,1);
      lcd.print((char) 255);
      lcd.print("Proximo");
      ultimoDadoExibido = dado;
      telaDeDadosCarregada = true;
    }else{
      if(!ultimoDadoExibido.equals(dado)){
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,0);
        lcd.print(buff);
        ultimoDadoExibido = dado;
      }
    }
}

void printaTelinha(char linha1[], char linha2[], bool selecaoLinha0, bool mostraEsquerda,  bool clicavel, bool mostraDireita){
    lcd.clear();
    if(selecaoLinha0){
      if(mostraEsquerda){
        lcd.print("<");
      }else{
        lcd.print(" ");
      }
      if(clicavel){
        lcd.print((char)255);
      }else{
        lcd.print("-");
      }

      if(mostraDireita){
        lcd.print(">");
      }else{
        lcd.print(" ");
      }
      lcd.print(linha1);
      lcd.setCursor(0,1);
      lcd.print("   ");
      lcd.print(linha2);
    }else{
      lcd.print("   ");
      lcd.print(linha1);
      lcd.setCursor(0,1);
      if(mostraEsquerda){
        lcd.print("<");
      }else{
        lcd.print(" ");
      }
      if(clicavel){
        lcd.print((char)255);
      }else{
        lcd.print("-");
      }
      if(mostraDireita){
        lcd.print(">");
      }else{
        lcd.print(" ");
      }
      lcd.print(linha2);      
    }
}

void trataNumero(int n){
        Serial.println("Entrou no tratador");
  if(taNoModoInput){
    if(calibNum1 == -1){
      Serial.println("setou num1");
      calibNum1 = n;
    }else{
      if(calibNum2 == -1){
        Serial.println("setou num2");
        calibNum2 = n;        
      }
    }
  }
}


void startScreen(){
   byte x = 9;
  for(int cont=0; cont < 5; cont++ ){
      for(int cont2 = 0; cont2 < cont; cont2 ++){
        bitmap.line(x-cont2, cont-cont2, x-cont2, cont+1-cont2 , true, UPDATE);
        bitmap.line(x+cont2, cont-cont2, x+cont2, cont+1-cont2 , true, UPDATE);                
      }
  //  bitmap.line(x, cont, x, cont+4 , true, UPDATE);
    delay(1);
  }
    bitmap.rectFill(0,7,25,8, true, UPDATE);
  //  bitmap.rectFill(8,8,11,11, true, UPDATE);
    for(int cont=0; cont < 3; cont++ ){
      for(int cont2 = 0; cont2 < cont; cont2 ++){
        bitmap.line(x-cont2, cont-cont2+8, x-cont2, cont+1-cont2+8 , true, UPDATE);
        bitmap.line(x+cont2, cont-cont2+8, x+cont2, cont+1-cont2+8 , true, UPDATE);                
      }
  //  bitmap.line(x, cont, x, cont+4 , true, UPDATE);
    delay(1);
  }
    lcd.setCursor(0, 0);
    lcd.print("TELEMETRIA");  
    lcd.setCursor(0,1);
    delay(800);
    lcd.print("Ceu Azul");
    delay(1000);
    bitmap.clear_text();
    lcd.setCursor(0,0);
    lcd.print("TELEMETRIA"); 
    lcd.setCursor(0,1);
    lcd.print("pera ai...");
}

void verificaControleIR(){
  if (irrecv.decode(&results))  {  
 //   Serial.print("Valor lido : ");  
 //   Serial.println(results.value, HEX);  
    armazenavalor = (results.value);  
    switch (armazenavalor) {
        case IR_0:
          Serial.println("0"); 
          trataNumero(0);
          break;
        case IR_1:
          Serial.println("1"); 
          trataNumero(1);
          break;
        case IR_2:
          Serial.println("2"); 
          trataNumero(2);
          break;
        case IR_3:
          Serial.println("3"); 
          trataNumero(3);
          break;
        case IR_4:
          Serial.println("4"); 
          trataNumero(4);
          break;
        case IR_5:
          Serial.println("5"); 
          trataNumero(5);
          break;
        case IR_6:
          Serial.println("6"); 
          trataNumero(6);
          break;
        case IR_7:
          Serial.println("7"); 
          trataNumero(7);
          break;
        case IR_8:
          Serial.println("8"); 
          trataNumero(8);
          break;
        case IR_9:
          Serial.println("9"); 
          trataNumero(9);
          break;
        case IR_OK:
          menu.use();
          Serial.println("OK"); 
          break;
        case IR_CIMA:
          Serial.println("CIMA"); 
          menu.moveUp();
          break;
        case IR_BAIXO:
          Serial.println("BAIXO"); 
          menu.moveDown();
          break;
        case IR_ESQUERDA:
          Serial.println("ESQUERDA"); 
          menu.moveLeft();
          break;
        case IR_DIREITA:
          Serial.println("DIREITA"); 
          menu.moveRight();
          break;
        case IR_LIGA:
          Serial.println("LIGA"); 
          break;
        case IR_MENOS:
          Serial.println("MENOS"); 
          break;
        case IR_PLAY:
          Serial.println("PLAY"); 
          break;
        case IR_PAUSE:
          Serial.println("PAUSE"); 
          break;
        case IR_STOP:
          Serial.println("STOP"); 
          break;
        case IR_VOLTA:
          Serial.println("VOLTA"); 
          break;
        default:
          break;
        //  Serial.println("merda"); 
    }

  irrecv.resume(); //Le o próximo valor  
  }  




}