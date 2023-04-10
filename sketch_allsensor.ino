

#include <OneWire.h>
#include <DallasTemperature.h>
#define TdsSensorPin A1
#define VREF 5.0             
#define SCOUNT  30 
#define SensorPin A0
#include <EEPROM.h>
#include "GravityTDS.h"
int rainAnalogPin = A2;
int rainDigitalPin = 3;


GravityTDS gravityTds;

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

unsigned long int avgValue;  //Armazene o valor médio do feedback do sensor
float b;
int buf[10],temp;


float temperature = 22,tdsValue = 0;

void setup(void)
{
 sensors.begin();  // Start up the library
 Serial.begin(115200);
  
 gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization

 pinMode(rainAnalogPin, INPUT);
  pinMode(rainDigitalPin, INPUT);
    
 
}

void loop(void)
{ 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
  
  delay(1000);

    

    {
  for(int i=0;i<10;i++)       //Obtenha 10 valores de amostra do sensor para suavizar o valor
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //classificar o analógico de pequeno para grande
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //pegue o valor médio de 6 amostras de centro
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //converter o analógico em milivolt
  phValue=3.5*phValue;                      //converter o milivolt em valor de pH
  Serial.print("pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(1000);
  digitalWrite(13, LOW);

  //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    delay(1000);

int sensorAnalogValue = analogRead(rainAnalogPin);
  Serial.print("Rain sensor analog value: ");
  Serial.println(sensorAnalogValue);
  
  // read the input on digital pin 2:
  int sensorDigitalValue = digitalRead(rainDigitalPin);
  
  
//digital output
 if(sensorAnalogValue >= 300) //High means rain detected; In some case vise versa
 {
 Serial.println("SEM CHUVA");
 }
 else if(sensorAnalogValue <= 250) 
 Serial.println("ESTA CHOVENDO");
 }
 
 delay(1000);
}
 

  
