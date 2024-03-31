// BLYNK - Tyto 4 věci musí být nahoře vždy--------------------- 
#define BLYNK_PRINT Serial                                  // |
                                                            // |  
#define BLYNK_TEMPLATE_ID "TMPLFj1tA8HB"                    // |
#define BLYNK_DEVICE_NAME "DevicesRodinnyDum"               // |
#define BLYNK_AUTH_TOKEN "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V" // |
//--------------------------------------------------------------

#include <WiFi.h>
#include <HTTPClient.h>

#include <BlynkSimpleEsp32.h>

const char* ssid = "Tp-ling";
const char* pass =  "hromasova";

char auth[] = "Fnk3Qt7yycOZWF-7TKymrCoZXHIOiF5V";
BlynkTimer timer;

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"

String serverName1 = "http://192.168.0.50/";
String serverName2 = "http://192.168.0.60/";
String serverName3 = "http://192.168.0.70/";
//String serverName4 = "http://192.168.137.80/";

int zar1,zar2,zar3;
int Err1,Err2,Err3;

float tep1, vlh1;
float tep2;
float tep3, volt3, amp3; 


void Data_Read_1();
void Data_Read_2();
void Data_Read_3();
void sendSensor_1();
void sendSensor_2();
void sendSensor_3();

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
  timer.setInterval(1000L, sendSensor_1);
  timer.setInterval(1250L, sendSensor_2);
  timer.setInterval(1500L, sendSensor_3);
  
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
    Err1 = 3;
    Err2 = 3;
    Err3 = 3;
    Data_Read_1();
    Data_Read_2();
    Data_Read_3();
    delay(1000);
  
  } 
}

void Data_Read_1(){
  
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
        
      }else if (Err1 == 0){
        Serial.println("Hodnoty: ");
        Serial.print("Zarizeni "); 
        Serial.println(zar1);
        Serial.print("Teplota: "); 
        Serial.print(tep1);
        Serial.println(" Stupnu Celsia, ");
        Serial.print("Vlhkost: "); 
        Serial.print(vlh1);
        Serial.println("%");
      }else{
        Serial.print("Zarizeni je offline");
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

void Data_Read_2(){
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName2);
    int httpResponseCode = http.GET();
    if(httpResponseCode>0){
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      String payload = http.getString();
      
      //--Zobrazení příchozího balíku
      //Serial.println(payload);

       zar2 = payload.substring(payload.indexOf('?')+1, payload.indexOf('%')).toInt();
       tep2 = payload.substring(payload.indexOf('%')+1, payload.indexOf('&')).toFloat();
       //vlh2 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toFloat();
       Err2 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toInt();
      
      if (Err2 == 1){
        Serial.println("Chyba senzoru"); 
        
      }else if (Err2 == 0){
        Serial.println("Hodnoty: ");
        Serial.print("Zarizeni "); 
        Serial.println(zar2);
        Serial.print("Teplota: "); 
        Serial.print(tep2);
        Serial.println(" Stupnu Celsia, ");
        //Serial.print("Vlhkost: "); 
        //Serial.print(vlh2);
        //Serial.println("%");
      }else{
        Serial.print("Zarizeni je offline");    
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

void Data_Read_3(){
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName3);
    int httpResponseCode = http.GET();
    if(httpResponseCode>0){
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      String payload = http.getString();
      
      //--Zobrazení příchozího balíku
      //Serial.println(payload);

       zar3 = payload.substring(payload.indexOf('?')+1, payload.indexOf('%')).toInt();
       tep3 = payload.substring(payload.indexOf('%')+1, payload.indexOf('&')).toFloat();
       volt3 = payload.substring(payload.indexOf('&')+1, payload.indexOf('#')).toFloat();
       amp3 = payload.substring(payload.indexOf('#')+1, payload.indexOf('$')).toFloat();
       Err3 = payload.substring(payload.indexOf('$')+1, payload.indexOf('!')).toInt();
      
      if (Err3 == 1){
        Serial.println("Chyba senzoru"); 
        
      }else if (Err3 == 0){
        Serial.println("Hodnoty: ");
        Serial.print("Zarizeni "); 
        Serial.println(zar3);
        
        Serial.print("Teplota: "); 
        Serial.print(tep3);
        Serial.println(" Stupnu Celsia, ");

        Serial.print("Napeti: "); 
        Serial.print(volt3);
        Serial.println(" V");

        Serial.print("Proud: "); 
        Serial.print(amp3);
        Serial.println(" A");

      }else{
        Serial.print("Zarizeni je offline");
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

void sendSensor_1(){
  WidgetLED led1(V11); 
  
  if(Err1 == 1){   
    led1.setColor(BLYNK_YELLOW);
    led1.on();
    delay(200);
    led1.off();
    delay(500);

  }else if (Err1 == 0){
    led1.setColor(BLYNK_GREEN);
    led1.on();
    delay(200);
    led1.off();
    delay(500);
  }else{
    led1.setColor(BLYNK_RED);
    led1.on();         
  }
    Blynk.virtualWrite(V12, tep1);
    Blynk.virtualWrite(V13, vlh1);

  if (Err1 != 3){
    Serial.print("-------------------------------------------------------------------------------> Zarizeni: ");Serial.print(zar1); Serial.println(" odeslano...."); 
  }else{
    Serial.println("-------------------------------------------------------------------------------> Zarizeni odojeno... ");   
  }
}

void sendSensor_2(){
  WidgetLED led2(V21); 
  
  if(Err2 == 1){   
    led2.setColor(BLYNK_YELLOW);
    led2.on();
    delay(200);
    led2.off();
    delay(500);

  }else if (Err2 == 0){
    led2.setColor(BLYNK_GREEN);
    led2.on();
    delay(200);
    led2.off();
    delay(500);
  }else{
    led2.setColor(BLYNK_RED);
    led2.on();    
  }
    Blynk.virtualWrite(V22, tep2);

  if (Err2 != 3){
    Serial.print("-------------------------------------------------------------------------------> Zarizeni: ");Serial.print(zar2); Serial.println(" odeslano...."); 
  }else{
    Serial.println("-------------------------------------------------------------------------------> Zarizeni odojeno... ");   
  }  
}

void sendSensor_3(){
  WidgetLED led3(V31); 
  
  if(Err3 == 1){   
    led3.setColor(BLYNK_YELLOW);
    led3.on();
    delay(200);
    led3.off();
    delay(500);

  }else if (Err3 == 0){
    led3.setColor(BLYNK_GREEN);
    led3.on();
    delay(200);
    led3.off();
    delay(500);
  }else{
    led3.setColor(BLYNK_RED);
    led3.on();    
  }
    Blynk.virtualWrite(V32, tep3);
    Blynk.virtualWrite(V33, volt3);
    Blynk.virtualWrite(V34, amp3);

  if (Err3 != 3){
    Serial.print("-------------------------------------------------------------------------------> Zarizeni: ");Serial.print(zar3); Serial.println(" odeslano...."); 
  }else{
    Serial.println("-------------------------------------------------------------------------------> Zarizeni odojeno... ");   
  }  
}

void loop() {
  //NIC TADY
}
