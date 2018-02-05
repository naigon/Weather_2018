#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

// Pin definitions
# define Hall sensor 2        //  Pino digital 2

// Constants definitions
const float pi = 3.14159265;           // Numero pi
int period = 5000;               // Tempo de medida(miliseconds)
int radius = 147;      // Raio do anemometro(mm)

// Variable definitions

unsigned int counter = 0; // magnet counter for sensor
unsigned int RPM = 0;          // Revolutions per minute
float speedwind = 0;         // Wind speed (m/s)


void setup()
{
  //lcd.begin (16,2);
  // Set the pins
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);     //internall pull-up active
    
  //Start serial 
  //Serial.begin(9600);       // sets the serial port to 9600 baud
  }

void loop()
{

  windvelocity();           
  
//*****************************************************************
//print km/h  
  SpeedWind();
  //lcd.setCursor(3,0);
  Serial.print("Vel. Vento: ");
  //lcd.setCursor(3,1);
  Serial.print(speedwind);
  Serial.println("Km/h");
  //Serial.print(" [km/h] ");  
  //Serial.println();


  delay(500);                        //taxa de atualização
}



// Measure wind speed
void windvelocity(){
  speedwind = 0;
  
  counter = 0;  
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();       
  long startTime = millis();
  while(millis() < startTime + period) {
  }
}

void SpeedWind(){
  int RPM = ((counter)*60)/(period/1000);
  speedwind = (((4 * pi * radius * RPM)/60) / 1000)*3.6;  // Calculate wind speed on km/h
 
}

void addcount(){
  counter++;
} 

