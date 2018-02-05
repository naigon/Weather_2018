#include <SoftwareSerial.h>
SoftwareSerial Ab(1,0);
#include <Wire.h>


void setup() {
Ab.begin(9600);
Serial.begin(9600);

}

void loop() {
  if (Ab.available() > 0) {
    String comando = Ab.readString();
    Serial.print(comando);
  }
  
}
