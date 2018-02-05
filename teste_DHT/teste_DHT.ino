#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
#define DHTPIN 8 // pino que estamos conectado
#define DHTTYPE DHT22 // DHT22
 
DHT dht(DHTPIN, DHTTYPE);
// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

void setup() 
{
  lcd.begin (16,2);
  dht.begin();
}
 
void loop() 
{
  delay(3000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) {
    return;
  } 
  else{
    lcd.setCursor(2,0);
    lcd.print("Umid: ");
    lcd.print(h);
    lcd.print(" %");
    lcd.setCursor(2,1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" *C");
  }
}
