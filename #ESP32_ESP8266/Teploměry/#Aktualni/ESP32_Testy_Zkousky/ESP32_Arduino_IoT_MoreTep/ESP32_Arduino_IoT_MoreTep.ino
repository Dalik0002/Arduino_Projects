#include <WiFi.h>
#include <HTTPClient.h>

#include "thingProperties.h"

char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";

const char* ssid = "DESKTOP-IF5CDUO";
const char* pass =  "123456789";

String serverName1 = "http://192.168.137.50/";
String serverName2 = "http://192.168.137.60/";
String serverName3 = "http://192.168.137.70/";
String serverName4 = "http://192.168.137.80/";

int zar1,zar2,zar3,zar4;
int Err1,Err2,Err3,Err4;

/*
float tep1, vlh1;
float tep2, vlh2;
float tep3, vlh3;
float tep4, vlh4;  
*/

void sendSensor();
void WiFi_Begin();
void Data_Read();

void setup() {
  Serial.begin(115200);
  WiFi_Begin(); 
  
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information youâll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}


void loop() {

  Data_Read();
  
  if (Err1 == 1){
    LED_Online_1 = false;
    LED_Err_1 = true;
    ArduinoCloud.update();
    delay(500);
  }
  else{
    LED_Err_1 = false;
    LED_Online_1 = true;
    delay(1000);
    ArduinoCloud.update();
    LED_Online_1 = false;
    delay(1000);
    ArduinoCloud.update();
    LED_Online_1 = true;
    delay(1000);
    ArduinoCloud.update();
  }
  //ArduinoCloud.update();
  //delay(500);

}

void WiFi_Begin(){
  WiFi.begin(ssid, pass);
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
      //Serial.println(payload);
      
      zar1 = payload.substring(payload.indexOf('?')+1, payload.indexOf('%')).toInt();
      tep1 = payload.substring(payload.indexOf('%')+1, payload.indexOf('&')).toFloat();
      vlh1 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toFloat();
      Err1 = payload.substring(payload.indexOf('#')+1, payload.indexOf('!')).toInt();

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
