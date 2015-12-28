#include <LiquidCrystal.h>
#include <LCDBitmap.h>

LiquidCrystal lcd(22, 24, 26, 28, 30, 32, 34); //Configura os pinos do Arduino para se comunicar com o LCD

LCDBitmap bitmap(&lcd, 11, 0);

   
void setup() {
  lcd.begin(16,2);
  bitmap.begin();
//  lcd.setCursor(5, 0);
 // lcd.print("LCDBitmap");  
}

void loop() {
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
    delay(5000);
    bitmap.clear_text();
    bitmap.clear();

}
