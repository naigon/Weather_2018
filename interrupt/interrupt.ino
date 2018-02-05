
const byte interruptPin = 19;
int pulse_count=0;
float rain_count=0;

int val = 0;
int old_val = 0; 

void ler_pluviometro();

void setup() {
  //pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ler_pluviometro, HIGH);
  Serial.begin(9600);
}

void loop() {
  
  delay(500);

}

void ler_pluviometro() {
  
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
    Serial.print(pulse_count*0.25); 
    Serial.println(" mm");
    digitalWrite(interruptPin,LOW);
    delay(200);
    //digitalWrite(ledPin,LOW);
  }else {   
      
    old_val = val;              //If the status hasn't changed then do nothing

     } 
}
