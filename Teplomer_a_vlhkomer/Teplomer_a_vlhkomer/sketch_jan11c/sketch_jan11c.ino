#include <DHT.h>
#include <DHT_U.h>

                       

#define DHTPIN 2                      
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);    

float hum;                                    
float temp;                                   

void setup()
{
  Serial.begin(9600);                         
  dht.begin();                                
}

void loop()
{
    
  hum = dht.readHumidity();                 
  temp = dht.readTemperature();                
  // až do teď se v programu nic nezměnilo
  // pokud je hodnota "hum" nebo "temp" nan, tak:
  if (isnan(hum) || isnan(temp))             
  {
    // vypíšeme po sériové lince "Chyba cteni"
    Serial.println("Chyba cteni!");          
  }
  // pokud jsou obě hodnoty v pořádku, tak:
  else                                       
  {
    Serial.print("Vlhkost: ");                
    Serial.print(hum);
    Serial.print(" %, Teplota: ");
    Serial.print(temp);
    Serial.println(" Celsius");
  }
  delay(2000);                              
}
