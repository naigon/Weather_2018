#include <VirtualWire.h>

struct package
{
  float temperature ;
  float humidity ;
  float rain;
  float wind;
  float luminosity;
  float count = 0.0;
};

typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(9600);
  //Define o pino 8 do Arduino como 
  //o pino de dados do transmissor
  vw_set_tx_pin(9);
  
  vw_setup(1000);   // Bits per sec
}

void loop()
{
data.temperature = 22.5;
data.humidity = 68.9;
data.rain = 0.85;
data.wind = 45.6;
data.luminosity = 84.0;
data.count = data.count+1;

//transmite os dados via RF
vw_send((uint8_t *)&data, sizeof(data));
vw_wait_tx(); // Aguarda o envio de dados
delay(2000);

} 


