 
#include <SdFat.h>
#include <SDHelper.h>
#include <Arduino.h>

SdFat SD;
File myFile;
File fileConfig;
int pino;

int calibra_ultrassomDist = -9999;
int calibra_pwm1Raw0 = -9999;
int calibra_pwm1RawMax = -9999;
int calibra_pwm1GrauMax = -9999;

int calibra_pwm2Raw0 = -9999;
int calibra_pwm2RawMax = -9999;
int calibra_pwm2GrauMax = -9999;

int calibra_pwm3Raw0 = -9999;
int calibra_pwm3RawMax = -9999;
int calibra_pwm3GrauMax = -9999;

int calibra_pwm4Raw0 = -9999;
int calibra_pwm4RawMax = -9999;
int calibra_pwm4GrauMax = -9999;

#define FILE_BASE_NAME "Telemetria - "
#define FILE_CONFIG_NAME "CONFIG.txt"
const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[] = FILE_BASE_NAME "00.csv";

SDHelper::SDHelper(int p){
  pino = p;
  pinMode(pino, OUTPUT);
  Serial.begin(9600);


}


void SDHelper::escreveDadoSD(float dado, int size){  // if the file opened okay, write to it:
  char charVal[20];                                  // temporarily holds data from vals 

  dtostrf(dado, (size+5), size, charVal);  //4 is mininum width, 3 is precision; float value is copied onto buff

  if (myFile) {
    myFile.print(charVal);
  } else {
    // erro
  }
}

void SDHelper::escreveDadoSD(int dado){
  if (myFile) {
    myFile.print(String(dado));
  } else {
    // erro
  }
}

void SDHelper::escreveDadoSD(long dado){
  if (myFile) {
    myFile.print(String(dado));
  } else {
    // erro
  }
}

void SDHelper::escreveDadoSD(unsigned long dado){
  if (myFile) {
    myFile.print(String(dado));
  } else {
    // erro99
  }
}

void SDHelper::escreveDadoSD(double dado){
  // if the file opened okay, write to it:
  char charVal[10];               //temporarily holds data from vals 
  
  dtostrf(dado, 8, 4, charVal);  //4 is mininum width, 3 is precision; float value is copied onto buff
  
  if (myFile) {
    myFile.print(charVal);
  } else {
    // erro
  }
}




void SDHelper::escreveDadoSD(String dado){
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(dado);
  } else {
    // erro
  }
}

void SDHelper::escreveNovaLinhaSD(){
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("");
  } else {
    // erro
  }
}

bool SDHelper::temArquivoSD(){
  // if the file opened okay, write to it:
  if (myFile) {
    return true;
  } else {
    return false;
  }
}


void SDHelper::abreSD(){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fileName, FILE_WRITE);

}

void SDHelper::ligaSD(){
    if (!SD.begin(pino)) {
      return;
    }else{
      while (SD.exists(fileName)) {
        if (fileName[BASE_NAME_SIZE + 1] != '9') {
          fileName[BASE_NAME_SIZE + 1]++;
        } else if (fileName[BASE_NAME_SIZE] != '9') {
          fileName[BASE_NAME_SIZE + 1] = '0';
          fileName[BASE_NAME_SIZE]++;
        } else {
          ("Tele-"+String(millis())+".csv").toCharArray(fileName, sizeof(fileName));
        }
      }
    }

}

void SDHelper::finalizaSD(){
  if (myFile) {
    myFile.close();
  } else {
    // erro
  }
}

bool SDHelper::leConfigSD(){
  if(SD.begin(pino)){
    calibra_ultrassomDist = -9999;
    calibra_pwm1Raw0 = -9999;
    calibra_pwm1RawMax = -9999;
    calibra_pwm1GrauMax = -9999;

    calibra_pwm2Raw0 = -9999;
    calibra_pwm2RawMax = -9999;
    calibra_pwm2GrauMax = -9999;

    calibra_pwm3Raw0 = -9999;
    calibra_pwm3RawMax = -9999;
    calibra_pwm3GrauMax = -9999;

    calibra_pwm4Raw0 = -9999;
    calibra_pwm4RawMax = -9999;
    calibra_pwm4GrauMax = -9999;
     char character;
     String settingName;
     String settingValue;
     fileConfig = SD.open(FILE_CONFIG_NAME, FILE_READ);
     if (fileConfig) {
       while (fileConfig.available()) {
         character = fileConfig.read();
         while((fileConfig.available()) && (character != '[')){
            character = fileConfig.read();
         }
         character = fileConfig.read();
         while((fileConfig.available()) && (character != '=')){
           settingName = settingName + character;
           character = fileConfig.read();
         }
         character = fileConfig.read();
         while((fileConfig.available()) && (character != ']')){
           settingValue = settingValue + character;
           character = fileConfig.read();
         }
         if(character == ']'){
           
           /*
           //Debuuging Printing
           Serial.print("Name:");
           Serial.println(settingName);
           Serial.print("Value :");
           Serial.println(settingValue);
           */
           
           // Apply the value to the parameter
           applySetting(settingName,settingValue);
           // Reset Strings
           settingName = "";
           settingValue = "";
         }
       }
       // close the file:
       fileConfig.close();
       return true;
     } else {
       return false;
     }
   }else{
   }
 }



 void SDHelper::applySetting(String settingName, String settingValue) {
   if(settingName == "ultrassomDist") {
      calibra_ultrassomDist = settingValue.toInt();
   }
   if(settingName == "pwm1Raw0") {
      calibra_pwm1Raw0 = settingValue.toInt();
   }
   if(settingName == "pwm1RawMax") {
      calibra_pwm1RawMax = settingValue.toInt();
   }
   if(settingName == "pwm1GrauMax") {
      calibra_pwm1GrauMax = settingValue.toInt();
   }
   if(settingName == "pwm2Raw0") {
      calibra_pwm2Raw0 = settingValue.toInt();
   }
   if(settingName == "pwm2RawMax") {
      calibra_pwm2RawMax = settingValue.toInt();
   }
   if(settingName == "pwm2GrauMax") {
      calibra_pwm2GrauMax = settingValue.toInt();
   }
   if(settingName == "pwm3Raw0") {
      calibra_pwm3Raw0 = settingValue.toInt();
   }
   if(settingName == "pwm3RawMax") {
      calibra_pwm3RawMax = settingValue.toInt();
   }
   if(settingName == "pwm3GrauMax") {
      calibra_pwm3GrauMax = settingValue.toInt();
   }
   if(settingName == "pwm4Raw0") {
      calibra_pwm4Raw0 = settingValue.toInt();
   }
   if(settingName == "pwm4RawMax") {
      calibra_pwm4RawMax = settingValue.toInt();
   }
   if(settingName == "pwm4GrauMax") {
      calibra_pwm4GrauMax = settingValue.toInt();
   }
 }
   
   // converting string to Float
float SDHelper::toFloat(String settingValue){
   char floatbuf[settingValue.length()+1];
   settingValue.toCharArray(floatbuf, sizeof(floatbuf));
   float f = atof(floatbuf);
   return f;
}
   
long SDHelper::toLong(String settingValue){
  char longbuf[settingValue.length()+1];
  settingValue.toCharArray(longbuf, sizeof(longbuf));
  long l = atol(longbuf);
  return l;
}
   
   // Converting String to integer and then to boolean
   // 1 = true
   // 0 = false
bool SDHelper::toBoolean(String settingValue) {
   if(settingValue.toInt()==1){
     return true;
   } else {
     return false;
   }
}
 
void SDHelper::calibraUltrassomNoSD(int ultrassomDist){
    leConfigSD();
    escreveConfigSD(ultrassomDist, calibra_pwm1Raw0, calibra_pwm1RawMax, calibra_pwm1GrauMax, calibra_pwm2Raw0, calibra_pwm2RawMax, calibra_pwm2GrauMax, calibra_pwm3Raw0, calibra_pwm3RawMax, calibra_pwm3GrauMax, calibra_pwm4Raw0, calibra_pwm4RawMax, calibra_pwm4GrauMax);
}

void SDHelper::calibraPWMNoSD(int pwmNum, int raw0, int rawMax, int grauMax){
    leConfigSD();
    
    switch (pwmNum) {
      case 1:
        escreveConfigSD(calibra_ultrassomDist, raw0, rawMax, grauMax, calibra_pwm2Raw0, calibra_pwm2RawMax, calibra_pwm2GrauMax, calibra_pwm3Raw0, calibra_pwm3RawMax, calibra_pwm3GrauMax, calibra_pwm4Raw0, calibra_pwm4RawMax, calibra_pwm4GrauMax);
        break;
      case 2:
        escreveConfigSD(calibra_ultrassomDist, calibra_pwm1Raw0, calibra_pwm1RawMax, calibra_pwm1GrauMax, raw0, rawMax, grauMax, calibra_pwm3Raw0, calibra_pwm3RawMax, calibra_pwm3GrauMax, calibra_pwm4Raw0, calibra_pwm4RawMax, calibra_pwm4GrauMax);
        break;
      case 3:
        escreveConfigSD(calibra_ultrassomDist, calibra_pwm1Raw0, calibra_pwm1RawMax, calibra_pwm1GrauMax, calibra_pwm2Raw0, calibra_pwm2RawMax, calibra_pwm2GrauMax, raw0, rawMax, grauMax, calibra_pwm4Raw0, calibra_pwm4RawMax, calibra_pwm4GrauMax);
        break;
      case 4:
        escreveConfigSD(calibra_ultrassomDist, calibra_pwm1Raw0, calibra_pwm1RawMax, calibra_pwm1GrauMax, calibra_pwm2Raw0, calibra_pwm2RawMax, calibra_pwm2GrauMax, calibra_pwm3Raw0, calibra_pwm3RawMax, calibra_pwm3GrauMax, raw0, rawMax, grauMax);
        break;
      default: 
        // if nothing else matches, do the default
        // default is optional
        break;
    }
}

 // Writes A Configuration file
 void SDHelper::escreveConfigSD(int ultrassomDist, int pwm1Raw0, int pwm1RawMax, int pwm1GrauMax, int pwm2Raw0, int pwm2RawMax, int pwm2GrauMax, int pwm3Raw0, int pwm3RawMax, int pwm3GrauMax, int pwm4Raw0, int pwm4RawMax, int pwm4GrauMax) {
  if(SD.begin(pino)){
     // Delete the old One
     SD.remove(FILE_CONFIG_NAME);
     // Create new one

     fileConfig = SD.open(FILE_CONFIG_NAME, FILE_WRITE);
     // writing in the file works just like regular print()/println() function
    if(ultrassomDist != -9999){
     fileConfig.print("[");
     fileConfig.print("ultrassomDist=");
     fileConfig.print(ultrassomDist);
     fileConfig.println("]");
    }

     // PWM1 CALIBRAÇÃO ---------------------
    if(pwm1Raw0 != -9999){
     fileConfig.print("[");
     fileConfig.print("pwm1Raw0=");
     fileConfig.print(pwm1Raw0);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm1RawMax=");
     fileConfig.print(pwm1RawMax);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm1GrauMax=");
     fileConfig.print(pwm1GrauMax);
     fileConfig.println("]");
    }
     // PWM2 CALIBRAÇÃO ----------------------
    if(pwm2Raw0 != -9999){
     fileConfig.print("[");
     fileConfig.print("pwm2Raw0=");
     fileConfig.print(pwm2Raw0);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm2RawMax=");
     fileConfig.print(pwm2RawMax);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm2GrauMax=");
     fileConfig.print(pwm2GrauMax);
     fileConfig.println("]");
    }

     // PWM3 CALIBRAÇÃO ----------------------
    if(pwm3Raw0 != -9999){
     fileConfig.print("[");
     fileConfig.print("pwm3Raw0=");
     fileConfig.print(pwm3Raw0);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm3RawMax=");
     fileConfig.print(pwm3RawMax);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm3GrauMax=");
     fileConfig.print(pwm3GrauMax);
     fileConfig.println("]");
    }
     // PWM4 CALIBRAÇÃO ----------------------
    if(pwm4Raw0 != -9999){
     fileConfig.print("[");
     fileConfig.print("pwm4Raw0=");
     fileConfig.print(pwm4Raw0);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm4RawMax=");
     fileConfig.print(pwm4RawMax);
     fileConfig.println("]");

     fileConfig.print("[");
     fileConfig.print("pwm4GrauMax=");
     fileConfig.print(pwm4GrauMax);
     fileConfig.println("]");
    }
     fileConfig.close();
  }
 //Serial.println("Writing done.");
 }

  int SDHelper::getUltrassomDist() {
    return calibra_ultrassomDist;
  }

   int SDHelper::getPwm1Raw0() {
    return calibra_pwm1Raw0;
  }

   int SDHelper::getPwm1RawMax() {
    return calibra_pwm1RawMax;
  }

   int SDHelper::getPwm1GrauMax() {
    return calibra_pwm1GrauMax;
  }

   int SDHelper::getPwm2Raw0() {
    return calibra_pwm2Raw0;
  }

   int SDHelper::getPwm2RawMax() {
    return calibra_pwm2RawMax;
  }

   int SDHelper::getPwm2GrauMax() {
    return calibra_pwm2GrauMax;
  }

   int SDHelper::getPwm3Raw0() {
    return calibra_pwm3Raw0;
  }

   int SDHelper::getPwm3RawMax() {
    return calibra_pwm3RawMax;
  }

   int SDHelper::getPwm3GrauMax() {
    return calibra_pwm3GrauMax;
  }

   int SDHelper::getPwm4Raw0() {
    return calibra_pwm4Raw0;
  }

   int SDHelper::getPwm4RawMax() {
    return calibra_pwm4RawMax;
  }

   int SDHelper::getPwm4GrauMax() {
    return calibra_pwm4GrauMax;
  }