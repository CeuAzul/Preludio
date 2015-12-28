#include <Csuporte.h>
#include <SoftwareSerial.h>
#include <SensorGPS.h>


SensorGPS gps;

void setup(){
  Serial.begin(9600);
}

void loop(){
   gps.atualizaGPS();

   Serial.print(gps.getNumSat());    
   Serial.print("  ");
   Serial.print(gps.getPrecisao());     
   Serial.print("  ");
   Serial.print(gps.getLatitude(), 11);        
   Serial.print("  ");
   Serial.print(gps.getLongitude(), 11); 
   Serial.print("  ");
   Serial.print(gps.getAge());    
   Serial.print("  ");
   Serial.print(gps.getAno());    
   Serial.print("  ");
   Serial.print(gps.getMes());     
   Serial.print("  ");
   Serial.print(gps.getDia());        
   Serial.print("  ");
   Serial.print(gps.getHora()); 
   Serial.print("  ");
   Serial.print(gps.getMinuto());   
   Serial.print("  ");
   Serial.print(gps.getSegundo());     
   Serial.print("  ");
   Serial.print(gps.getCentSegundo());        
   Serial.print("  ");
   Serial.print(gps.getAltitude(), 4);   
   Serial.print("  ");
   Serial.print(gps.getCurse(), 4);     
   Serial.print("  ");
   Serial.print(gps.getVelocidade(), 4);        
   Serial.print("  ");
   Serial.print(gps.getX(), 4);   
   Serial.print("  ");
   Serial.print(gps.getY(), 4);     
   Serial.print("  ");
   Serial.print(gps.getDist(), 4);        
   Serial.print("  ");
   Serial.print(gps.getLatRef(), 11);     
   Serial.print("  ");
   Serial.println(gps.getLongRef(), 11);        

   

   



  Serial.println();
  
  gps.smartdelay(600);
}
