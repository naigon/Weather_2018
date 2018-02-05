//Nelson Weather
//Estação completa com todos os sensores e transmissao via XBEE

#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

RTC_DS1307 rtc;


// Variáveis/constantes pluviometro:
const byte interruptPin = 19;
int pulse_count=0;
float rain_count=0;
int val = 0;
int old_val = 0;

// Variáveis/constantes biruta:
#define pin_biruta 0
float valor =0;
int dir_graus =0;
String dir = "";

// Variáveis/constantes anemometro:
// Constants definitions anem.
const float pi = 3.14159265;           // Numero pi
int period = 5000;               // Tempo de medida(miliseconds) 5000
int radius = 147;      // Raio do anemometro(mm)
//variaveis
unsigned int counter = 0; // magnet counter for sensor
unsigned int RPM = 0;          // Revolutions per minute
float vel_vento = 0; 

//definições sensor temp/umid
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);// Inicializa o display no endereco 0x3F

//simbolos usados na amostragem do display
byte grau[8] = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};
byte termometro[8] = {B00100,B01010,B01010,B01010,B01110,B11111,B11111,B01110};
byte gota[8] = {B00100,B00100,B01010,B01010,B10001,B10001,B10001,B01110};
byte sol[8] = {B00100,B10101,B01110,B11011,B01110,B10101,B00100,B00000};
byte barra[8] = {B00100,B00000,B00100,B00000,B00100,B00000,B00100,B00000};


//declaracao dos prototipos das funcoes
void mostra_display(float temp, float humid, int lumin, float vento, float chuva);
void grava_cartao_SD(String backup);
void inicializa_DataLogger(byte led1, byte led2);
void ler_pluviometro();
void ler_biruta();
void windvelocity();
void SpeedWind();
void RPMcalc();
void addcount();

//==========================================================================================================================

void setup(){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ler_pluviometro, HIGH);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);     //internall pull-up active
  //leds do protoshield
  byte LED1 = 6;
  byte LED2 = 7;
  pinMode(A3, INPUT);// pino analogico para leitura do LDR
  pinMode(LED1, OUTPUT);// led de verificação do cartao SD
  pinMode(LED2, OUTPUT);//led de verificação do RTC
  //String ssid = "Bandidos Molhados";
  //String senha = "maisde8000";  
  Serial2.begin(9600);//inicializa comunicação serial ARDUINO/ESP8266
  Serial.begin(9600);//inicializa comunicação serial PC/ARDUINO
  
  inicializa_DataLogger(LED1, LED2);
  
  dht.begin();//inicializa sensor temperatura/umidade dht22
  lcd.begin(16, 2);//inicializa display LCD 16x2
  delay(400);

  //inicializa simbolos usados no display
  lcd.createChar(0, termometro);
  lcd.createChar(1, gota);
  lcd.createChar(2, sol);
  lcd.createChar(4, grau);
  lcd.createChar(5, barra);
    
}

//==========================================================================================================================

void loop(){

  windvelocity();
  RPMcalc();          
  SpeedWind();
  ler_biruta();
  ler_pluviometro();
  
  int luminosidade = 0;
  luminosidade = analogRead(A3);//le a porta analogica ligada ao LDR
  luminosidade = map(luminosidade,0,1023,99,0);//mapeia valores para 0-99% (100% nao cabe no display :D )

  float humid = dht.readHumidity();//le nivel de umidade
  float temp = dht.readTemperature();//le temperatura em graus celsius
  
  //verifica se as leituras foram validas, se nao sai do laço e tenta de novo
  if (isnan(humid) || isnan(temp)) {
    return;
  }
  
  mostra_display(temp, humid, luminosidade, vel_vento, rain_count);
  grava_cartao_SD(String(temp)+','+String(humid)+','+String(luminosidade)+','+String(vel_vento)+','+String(rain_count)+','+dir);

  //envia para o xbee
  Serial2.print(String(temp)+','+String(humid)+','+String(luminosidade)+','+String(vel_vento)+','+String(rain_count));

  
  delay(3000);
  
}

//==========================================================================================================================

void mostra_display(float temp, float humid,int lumin, float vento, float chuva){
  lcd.clear();
  //atualiza luminosidade no display LCD
  lcd.setCursor(13,1);
  lcd.print(lumin);
  lcd.setCursor(14,0);
  lcd.write(byte(2));
  lcd.setCursor(15,1);
  lcd.print(F("%"));
  
  //atualiza temperatura no display LCD
  lcd.setCursor(3,0);
  lcd.write(byte(0));
  lcd.setCursor(6,0);
  lcd.write(byte(5));
  lcd.setCursor(6,1);
  lcd.write(byte(5));
  lcd.setCursor(0,1);
  lcd.print(temp, 1);
  lcd.write(byte(4));
  lcd.print(F("C"));

  //atualiza umidade no display LCD
  lcd.setCursor(9,0);
  lcd.write(byte(1));
  lcd.setCursor(12,0);
  lcd.write(byte(5));
  lcd.setCursor(12,1);
  lcd.write(byte(5));
  lcd.setCursor(7,1);
  lcd.print(humid, 1);
  lcd.print(F("%"));
  
  delay(5000);
  //segunda tela do display lcd

  //velocidade do vento (Km/h)
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(F("Vento:"));
  lcd.setCursor(0,1);
  lcd.print(vento);
  lcd.print(F("Km/h"));

  //separador
  lcd.setCursor(8,0);
  lcd.write(byte(5));
  lcd.setCursor(8,1);
  lcd.write(byte(5));

  //nivel de chuva (mm)
  lcd.setCursor(10,0);
  lcd.print(F("Chuva:"));
  lcd.setCursor(9,1);
  lcd.print(chuva);
  lcd.print(F("mm"));
   }

//==========================================================================================================================

void grava_cartao_SD(String backup){
  
  File file;
  //Abre arquivo no SD para gravacao
  file = SD.open("log.txt", FILE_WRITE);
  //Le as informacoes de data e hora
  DateTime now = rtc.now();
  //Grava os dados no cartao SD
  file.print(F("Data/hora: "));
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
  file.print(F("|Temperatura|Umidade|Luminosidade|Vel.Vento|Niv.Chuva| : "));
  file.println(backup);
  //Fecha arquivo
  file.close();
}

//==========================================================================================================================

void inicializa_DataLogger(byte led1, byte led2){
  //Inicia o cartao SD
  Serial.println(F("Iniciando cartao SD..."));
  if (!SD.begin(10 , 11 , 12 , 13))
  {
    Serial.println(F("Falha na inicializacao do SD!"));
    return;
  }
  Serial.println(F("Cartao SD iniciado.\n"));
  digitalWrite(led1, HIGH);

  //Verifica as condicoes do RTC
  if (! rtc.begin())
  {
    Serial.println(F("RTC nao encontrado!"));
    while (1);
  }
  if (rtc.isrunning()){
    
    Serial.println(F("RTC funcionando."));
    digitalWrite(led2, HIGH);
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

void ler_pluviometro(){
  val =  digitalRead(interruptPin);
  if ((val == LOW) && (old_val == HIGH)){
    delay(10);
    //digitalWrite (ledPin,HIGH);   //LED for visual feedback that pulses from rain gauge are being received
    pulse_count = pulse_count + 1; //add up the total amount of rain that has fallen and put it in variable totalAmount
    old_val = val;
    Serial.print("Medida de chuva (contagem): ");
    Serial.print(pulse_count);//*0.2794); 
    Serial.println(" pulso");
    Serial.print("Medida de chuva (calculado): ");
    rain_count = pulse_count*0.25;
    Serial.print(rain_count); 
    Serial.println(" mm");
    digitalWrite(interruptPin, LOW);
    delay(200);
    //digitalWrite(ledPin,LOW);
  }else {   
      
    old_val = val;              //If the status hasn't changed then do nothing

     } 
}

void ler_biruta(){
  valor = analogRead(pin_biruta)* (5.0 / 1023.0);
 
if (valor <= 0.27) {
dir_graus = 315;
dir = "NO";
}
else if (valor <= 0.32) { 
dir_graus = 270;
dir = "O";
}
else if (valor <= 0.38) { 
dir_graus = 225;
dir = "SO";
}
else if (valor <= 0.45) { 
dir_graus = 180;
dir = "S";
}
else if (valor <= 0.57) { 
dir_graus = 135;
dir = "SE";
}
else if (valor <= 0.75) { 
dir_graus = 90;
dir = "E";
}
else if (valor <= 1.25) {  
dir_graus = 45;
dir = "NE";
}
else {  
dir_graus = 0;
dir = "N";
}

 Serial.print("Direcao: ");
 Serial.print(dir_graus);
 Serial.println(" graus");
 Serial.print("Apontando p/: ");
 Serial.println(dir);
 
 delay (1000);
 Serial.println();
  }

// Measure wind speed
void windvelocity(){
  vel_vento = 0;
  counter = 0;  
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();       
  long startTime = millis();
  while(millis() < startTime + period) {
  }
}


void SpeedWind(){
  vel_vento = (((4 * pi * radius * RPM)/60) / 1000)*3.6;  // Calculate wind speed on km/h
  Serial.print("Velocidade do vento: ");
  Serial.print(vel_vento);
  Serial.println("Km/h");
 
}

void RPMcalc(){
  RPM=((counter)*60)/(period/1000);  // Calculate revolutions per minute (RPM)
}

void addcount(){
  counter++;
} 
