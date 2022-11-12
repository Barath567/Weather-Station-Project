#include <SoftwareSerial.h>
#include <dht.h>
#define RX 2
#define TX 3
dht DHT;
float humidity;
float temp;
#define dht_apin A0 
String AP = "Yathu Cat";       // AP NAME
String PASS = "IamYathu"; // AP PASSWORD
String API = "GVDMM1ZC55MCQDDG";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
const int capteur_D = 0;
#include "TimerOne.h"
unsigned int counter=0;
void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
} 
int val_analogique;
#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 pressure;
#define ALTITUDE 8.0
SoftwareSerial esp8266(RX,TX); 
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
pressure.begin();

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue()+"&field3="+getGasvalue()+"&field4="+getpressurevalue()+"&field5="+getrainvalue();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String getTemperatureValue(){

    DHT.read11(dht_apin); //A0,5v,gnd
  float temp=DHT.temperature;
   
   return String(temp); 
  
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String getHumidityValue(){
    DHT.read11(dht_apin); //A0,5v,gnd
  float humidity=DHT.humidity;
   
   return String(humidity); 
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String getGasvalue(){
  
  float sensorvalue = analogRead(A3);
  
  return String(sensorvalue); 
  }
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String getrainvalue(){

  val_analogique=analogRead(A2);
  
  return String(val_analogique);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String getpressurevalue() {

  char status;
  double T,P,p0,a;  
  Serial.println();
  status = pressure.startPressure(4);
  status = pressure.getPressure(P,T);

 
  return String(P);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Done");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
