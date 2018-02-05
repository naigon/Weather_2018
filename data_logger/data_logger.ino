//Programa: Data logger

#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#define DHTPIN 8     // what digital pin we're connected to
#define DHTTYPE DHT22  
#define LED1 6
#define LED2 7

DHT dht(DHTPIN, DHTTYPE);

//Pino CS do cartao SD

int cont = 0;
RTC_DS1307 rtc;

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);

  dht.begin();
  Serial.println(F("Testando Shield Data Logger com DHT22"));
  Serial.println();

  //Inicia o cartao SD
  Serial.println(F("Iniciando cartao SD..."));
  if (!SD.begin(10 , 11 , 12 , 13))
  {
    Serial.println(F("Falha na inicializacao do SD!"));
    return;
  }
  Serial.println(F("Cartao SD iniciado."));
  digitalWrite(LED1, HIGH);
  Serial.println();

  //Verifica as condicoes do RTC
  if (! rtc.begin())
  {
    Serial.println(F("RTC nao encontrado!"));
    while (1);
  }
  if (rtc.isrunning()){
    
    Serial.println(F("RTC funcionando."));
    digitalWrite(LED2, HIGH);
  }
  else{
    Serial.println(F("RTC nao operante!"));
    //A linha abaixo ajusta o RTC com a data e hora do momento da compilacao
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //A linha abaixo permite ajustar manualmente a data e hora do RTC
    //Formato: DateTime(ano, mes, dia, hora, minuto, segundo)
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
    
}

void loop(void)
{
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  grava_cartao_SD(String(t)+" "+String(h)+" "+cont);
  cont=cont+1;
  delay(1000);

}

void grava_cartao_SD(String teste)
{
  File file;
  //Abre arquivo no SD para gravacao
  file = SD.open("log.txt", FILE_WRITE);
  //Le as informacoes de data e hora
  DateTime now = rtc.now();
  //Grava os dados no cartao SD
  file.print("Data/hora: ");
  Serial.print(now.day() < 10 ? "0" : "");
  file.print(now.day(), DEC);
  file.print('/');
  file.print(now.month() < 10 ? "0" : "");
  file.print(now.month(), DEC);
  file.print('/');
  file.print(now.year(), DEC);
  file.print(' ');
  file.print(now.hour() < 10 ? "0" : "");
  file.print(now.hour(), DEC);
  file.print(':');
  file.print(now.minute() < 10 ? "0" : "");
  file.print(now.minute(), DEC);
  file.print(':');
  file.print(now.second() < 10 ? "0" : "");
  file.print(now.second(), DEC);
  file.print(' ');
  file.print(F("- Amostra Temperatura/Umidade: "));
  file.println(teste);
  //Fecha arquivo
  file.close();
}
