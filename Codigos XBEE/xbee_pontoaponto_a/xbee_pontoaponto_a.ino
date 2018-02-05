#include <SoftwareSerial.h>
SoftwareSerial Ab(1,0);

int count = 0;

void setup() {
 Ab.begin(9600);
}

void loop() {
  Ab.print("23.52,88.61,75.1,14.66,2.55"+String(count));
  count++;
  delay(2000);  
}
