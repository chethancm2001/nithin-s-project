#include "DHT.h"
#include <ESP8266WiFi.h>

DHT dht = DHT(D1, DHT11); 

String apiKey = "O5VURA2CU9BGUQB1";

const char *ssid =  "miniproject";     
const char *pass =  "miniproject";
const char *server = "api.thingspeak.com";

WiFiClient client;

void setup()
{
Serial.begin(115200);
pinMode(D0,INPUT);
pinMode(D4,OUTPUT);
dht.begin();
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}

void loop()
{

delay(1000);
float humidity = dht.readHumidity(); 
float temperature = dht.readTemperature(); 
int soilmosture = digitalRead(D0);
Serial.print("Humidity=");
Serial.print(humidity);
Serial.println("  ");
Serial.print("temperature=");
Serial.println(temperature);
Serial.println(soilmosture);
//turning the motor on
if(soilmosture == 1){
  Serial.println("Water is less Turning on the motor");
  while(soilmosture == 1){
   soilmosture = digitalRead(D0);
   digitalWrite(D4,LOW);
  Serial.println("motor is on");
  }

  
  
}
else{
    digitalWrite(D4,HIGH);
}

//sending the data to things speak


 if (isnan(humidity) || isnan(temperature)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temperature);
                             postStr +="&field2=";
                             postStr += String(humidity);
                             postStr +="&field3=";
                             postStr += String(soilmosture);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             delay(1000);
                             Serial.println("data is sent");
 
                          
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  

}
