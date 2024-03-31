/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLFj1tA8HB"
#define BLYNK_DEVICE_NAME "PokusJedna"
#define BLYNK_AUTH_TOKEN "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V"

#include <WiFi.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";

const char* ssid = "DESKTOP-IF5CDUO";
const char* pass =  "123456789";

String serverName1 = "http://192.168.137.50/";

int zar1,zar2,zar3,zar4;

float tep1, vlh1;
float tep2, vlh2;
float tep3, vlh3;
float tep4, vlh4;  

void sendSensor();

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

void setup() {
  Serial.begin(115200);
   
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting.. Blynk");
  Blynk.begin(auth, ssid, pass);
  Serial.println("Started..");
}

void loop() {
  
  Blynk.run();
  
  Data_Read();
  sendSensor();

  delay(1000);
}

void sendSensor(){ 
  Serial.print("AM HERE");
  
  WidgetLED led1(V1);
  WidgetLED led2(V2);
  
    led2.off();
    led1.off();
    delay(100);
    led1.on();
    delay(100);  
    led1.off();
    delay(100);
    led1.on();
    
    Blynk.virtualWrite(V0, tep1);
    Blynk.virtualWrite(V3, vlh1);
}
