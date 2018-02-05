#include <Wire.h>
#include <Adafruit_INA219.h> // You will need to download this library

Adafruit_INA219 sensor219; // Declare and instance of INA219


  float shunt_voltage = 0;
  float bus_voltage = 0;
  float current = 0; // Measure in milli amps
  float power = 0; //em watts
  float load_voltage = 0;
  float power_mw = 0; //em mW


void leituras_INA219();

void setup(void) 
{
      
  Serial.begin(9600);    
  sensor219.begin();
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  sensor219.setCalibration_16V_400mA();
  
}

void loop(void) 
{
 
leituras_INA219();

}

void leituras_INA219(){
  
  bus_voltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA();
  shunt_voltage = sensor219.getShuntVoltage_mV();
  load_voltage = bus_voltage + (shunt_voltage / 1000);
  power = bus_voltage * (current/1000); // Calculate the Power em watts
  power_mw = bus_voltage * current; // Calculate the Power in mW
  
  
  Serial.print("Load Voltage:   "); 
  Serial.print(load_voltage); 
  Serial.println(" V");  
  
  Serial.print("Shunt Voltage:   "); 
  Serial.print(shunt_voltage); 
  Serial.println(" mV");  
  
  Serial.print("Bus Voltage:   "); 
  Serial.print(bus_voltage); 
  Serial.println(" V");  
  
  Serial.print("Current:       "); 
  Serial.print(current); 
  Serial.println(" mA");
  
  Serial.print("Power (W):         "); 
  Serial.print(power); 
  Serial.println(" W");  

  Serial.print("Power (mW):         "); 
  Serial.print(power_mw); 
  Serial.println(" mW");  
  
  Serial.println("");  

  delay(2000);
  
  
  }
