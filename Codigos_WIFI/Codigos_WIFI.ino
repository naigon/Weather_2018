#include <SoftwareSerial.h>
#include <SoftwareSerial.h>

boolean thingSpeakWrite(float value1, float value2, float value3, float value4, float value5);
String envia_dadosESP(String command, const int timeout, boolean debug);

//serial debug
#define DEBUG true

void setup() {
  Serial.begin(9600);//inicializa comunicação serial PC/ARDUINO
  Serial2.begin(9600);//inicializa comunicação serial ARDUINO/ESP8266
  delay(400);

  envia_dadosESP("AT+CWMODE=1\r\n", 2000, DEBUG);//seta esp8266 como cliente

  envia_dadosESP("AT+CWJAP=\""+String("Bandidos Molhados")+"\",\""+String("maisde8000")+"\"", 3000, DEBUG);// set your home router SSID and password
   if (DEBUG)  Serial.println(F("\n\nSETUP COMPLETO!\n")); 
  // put your setup code here, to run once:

}

void loop() {
  thingSpeakWrite(23.52, 88.61, 75.1, 14.66, 2.55);  // escreve os valores na aplicação thingspeak
  delay(10000);
  // put your main code here, to run repeatedly:

}

boolean thingSpeakWrite(float value1, float value2, float value3, float value4, float value5){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial2.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial2.find((char *)"Error")){
    if (DEBUG) Serial.println(F("AT+CIPSTART error"));
    return false;
  }
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += "BM0MQ915S0D8923Y";     // replace with your channel's thingspeak WRITE API key //esta linha nao pode ser otimizada
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr +="&field3=";
  getStr += String(value3);
  getStr +="&field4=";
  getStr += String(value4);
  getStr +="&field5=";
  getStr += String(value5);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial2.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(200);
  if(Serial2.find((char *)">")){
    Serial2.print(getStr);
    if (DEBUG) { //Serial.print(getStr+"Canal ThingSpeak atualizado com sucesso! "+"Amostra numero: "+"\n");
                  Serial.print(F("Canal ThingSpeak atualizado com sucesso! \n"));
                 }
  }
  else{
    Serial2.println(F("AT+CIPCLOSE"));
    // alert user
    if (DEBUG)   Serial.println(F("AT+CIPCLOSE"));
    return false;
  }
  return true;
}
//==========================================================================================================================

String envia_dadosESP(String command, const int timeout, boolean debug){
  
  // Envio dos comandos AT para o modulo
  String response = "";
  Serial2.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial2.available())
    {
      // The esp has data so display its output to the serial window
      char c = Serial2.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
