#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DESKTOP-IF5CDUO";
const char* password =  "123456789";

String serverName1 = "http://192.168.137.50/";


int zar1,zar2,zar3,zar4;

float tep1, vlh1;
float tep2, vlh2;
float tep3, vlh3;
float tep4, vlh4;  

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void Data_Read(){
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName1);
    int httpResponseCode = http.GET();
    if(httpResponseCode>0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      
      zar1 = payload.substring(payload.indexOf(';')+1, payload.indexOf('%')).toInt();
      tep1 = payload.substring(payload.indexOf('%')+1, payload.indexOf('&')).toFloat();
      vlh1 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toFloat();

      Serial.print("Zarizeni "); 
      Serial.println(zar1);
      Serial.print("Teplota: "); 
      Serial.print(tep1);
      Serial.println(" Stupnu Celsia, ");
      Serial.print("Vlhkost: "); 
      Serial.print(vlh1);
      Serial.println("%");
      
    }
    else{
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else{
    Serial.println("WiFi Disconnected");
  }
  
  }

void loop() {
  Data_Read();

  Serial.println("AM here");
  
  delay(1000);
}
