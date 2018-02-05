#include <VirtualWire.h>
#include <Wire.h>

byte message[VW_MAX_MESSAGE_LEN];    // Armazena as mensagens recebidas
byte msgLength = VW_MAX_MESSAGE_LEN; // Armazena o tamanho das mensagens

char temperatureChar[10];
char humidityChar[10];
char rainChar[10];
char windChar[10];
char luminosityChar[10];
char countChar[10];

struct package
{
  float temperature=0.0 ;
  float humidity=0.0 ;
  float rain=0.0;
  float wind=0.0;
  float luminosity=0.0;
  float count = 0.0;
};

typedef struct package Package;
Package data;



void setup()   {
  Serial.begin(9600);
  vw_set_rx_pin(5); // Define o pino 5 do Arduino como entrada 
//de dados do receptor
  vw_setup(1000);             // Bits por segundo
  vw_rx_start();              // Inicializa o receptor
  delay(2000);

}

void loop()
{

  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

if (vw_have_message())  // Is there a packet for us? 
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);

    String temperatureString = String(data.temperature,1);
    temperatureString.toCharArray(temperatureChar,10);
    Serial.print(temperatureChar);
    Serial.print(',');

    String humidityString = String(data.humidity,1);
    humidityString.toCharArray(humidityChar,10);
    Serial.print(humidityChar);
    Serial.print(',');

    String rainString = String(data.rain,2);
    rainString.toCharArray(rainChar,10);
    Serial.print(rainChar);
    Serial.print(',');

    String windString = String(data.wind,1);
    windString.toCharArray(windChar,10);
    Serial.print(windChar);
    Serial.print(',');

    String luminosityString = String(data.luminosity,0);
    luminosityString.toCharArray(luminosityChar,10);
    Serial.print(luminosityChar);
    Serial.print(',');

    
    String countString = String(data.count,0);
    countString.toCharArray(countChar,10);
    Serial.print(countChar);
   
  }
  
}

