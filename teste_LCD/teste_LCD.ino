// Programa : Display LCD 16x2 e modulo I2C

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
 
void setup()
{
 lcd.begin (16,2);
}
 
void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Seu cretino");
  lcd.setCursor(0,1);
  lcd.print("vou te pegar!");
 
  delay(1000);
  
}
