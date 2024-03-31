/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/a7d5fab8-1e67-4ec0-a3f9-357c76d78f04 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float tep;
  float vlh;
  bool lED_Status_1;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include <WiFi.h>
#include <HTTPClient.h>

#include "thingProperties.h"

/*
const char* ssid = "DESKTOP-IF5CDUO";
const char* pass =  "123456789";
*/

String serverName1 = "http://192.168.137.50/";
String serverName2 = "http://192.168.137.60/";
String serverName3 = "http://192.168.137.70/";
String serverName4 = "http://192.168.137.80/";

void Data_Read();
void sendSensor();

int zar1,zar2,zar3,zar4;
int Err1,Err2,Err3,Err4;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  Serial.begin(115200);
  delay(1000); 
  WiFi.begin(SSID, PASS);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

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
  
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  while(1){
    Data_Read();
    delay(1000);
    ArduinoCloud.update();
  
  } 
}

void Data_Read(){
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName1);
    int httpResponseCode = http.GET();
    if(httpResponseCode>0){
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      String payload = http.getString();
      
      //--Zobrazení příchozího balíku
      //Serial.println(payload);

       zar1 = payload.substring(payload.indexOf('?')+1, payload.indexOf('%')).toInt();
       tep1 = payload.substring(payload.indexOf('%')+1, payload.indexOf('&')).toFloat();
       vlh1 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toFloat();
       Err1 = payload.substring(payload.indexOf('#')+1, payload.indexOf('!')).toInt();
      
      if (Err1 == 1){
        Serial.println("Chyba senzoru"); 
        
      }else{
        Serial.println("Hodnoty: ");
        Serial.print("Zarizeni "); 
        Serial.println(zar1);
        Serial.print("Teplota: "); 
        Serial.print(tep1);
        Serial.println(" Stupnu Celsia, ");
        Serial.print("Vlhkost: "); 
        Serial.print(vlh1);
        Serial.println("%");
      }
      
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

//--------------------------------------------------------------------------------------------------------------------

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  while(1){
    sendSensor();
    ArduinoCloud.update();    
  }
}

void sendSensor(){
  if(Err1 == 1){  
    LED_Status_1 = false;
  }else{
    LED_Status_1 = true;
  }     
}

void loop() {
  //NIC TADY
}
