#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
//LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

byte grau[8] = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};
 
// Const def
#define pin_biruta 0
float valor =0;
int dir_graus =0;
String dir = "";

void setup() {
  //  inicializa a comunicação serial a 9600 bps: 
  Serial.begin(9600);
  //lcd.createChar(4, grau);
}

void loop() {
  
ler_biruta();
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
 
 Serial.print(" Direcao: ");
 Serial.print(dir_graus);
 Serial.print(" graus");
 
 
 Serial.print("Apontando p/: ");
 Serial.println(dir);
 delay (1000);
  }



