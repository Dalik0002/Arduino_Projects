// BLYNK - Tyto 4 věci musí být nahoře vždy--------------------- 
#define BLYNK_PRINT Serial                                  // |
                                                            // |  
#define BLYNK_TEMPLATE_ID "TMPLFj1tA8HB"                    // |
#define BLYNK_DEVICE_NAME "PokusJedna"                      // |
#define BLYNK_AUTH_TOKEN "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V" // |
//--------------------------------------------------------------

#include <WiFi.h>
#include <HTTPClient.h>

#include <BlynkSimpleEsp32.h>

const char* ssid = "DESKTOP-IF5CDUO";
const char* pass =  "123456789";

char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";
BlynkTimer timer;

String serverName1 = "http://192.168.137.50/";
String serverName2 = "http://192.168.137.60/";
String serverName3 = "http://192.168.137.70/";
String serverName4 = "http://192.168.137.80/";

int zar1,zar2,zar3,zar4;
int Err1,Err2,Err3,Err4;

float tep1, vlh1;
float tep2, vlh2;
float tep3, vlh3;
float tep4, vlh4; 

void Data_Read();
void sendSensor();

TaskHandle_t Task1;
TaskHandle_t Task2;

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

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  
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

//----------------------------------------------------------------------------

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  while(1){
    Blynk.run();

    timer.run();
  }
}

void sendSensor(){
  WidgetLED led1(V1);
  WidgetLED led2(V2); 
  
  if(Err1 == 1){  
    led2.on();
    led1.off();
    delay(100);
    led2.off();
    delay(100);  
    led2.on();
    delay(100);
    led2.off();
    
    }else{
      
      led2.off();
      led1.off();
      delay(100);
      led1.on();
      delay(100);  
      led1.off();
      delay(100);
      led1.on();
    }
      Blynk.virtualWrite(V0, tep1);
      Blynk.virtualWrite(V3, vlh1);

      Serial.println("Sent..."); 
      
  }

void loop() {
  //NIC TADY
}
