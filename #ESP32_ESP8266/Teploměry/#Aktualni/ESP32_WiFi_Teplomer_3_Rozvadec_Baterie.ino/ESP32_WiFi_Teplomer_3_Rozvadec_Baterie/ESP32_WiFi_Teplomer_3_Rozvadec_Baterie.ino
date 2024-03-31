////////////////////////////////////////////////////////////////
//                                                            //
//   ESP32 - DHT11 senzor, Proudový senzor, Napěťový senzor   //
//            (Rozvaděč ve sklepě u baterek)                  //
//                                                            //
////////////////////////////////////////////////////////////////


#include <WiFi.h>
#include <HTTPClient.h>

#include <DHT.h>

#include <ACS712.h>
//#include <ESP8266WebServer.h>

#define zarizeni 3

#define Current_Pin 7

// https://miliohm.com/acs712-current-sensor-arduino/ 

ACS712 sensor(ACS712_20A, Current_Pin);
//ACS712_20A for 20 Amp type
//ACS712_30A for 30 Amp type

/*---------------------------------https://github.com/BasOnTech/Arduino-Beginners-EN/blob/master/E17-voltage-sensor/voltage-sensor.ino-----------NAPĚTÍ

const int voltageSensorPin = A0;          // sensor pin
float vIn;                                // measured voltage (3.3V = max. 16.5V, 5V = max 25V)
float vOut;
float voltageSensorVal;                   // value on pin A3 (0 - 1023)
const float factor = 5.128;               // reduction factor of the Voltage Sensor shield
const float vCC = 3.30;

---------------------------------------------------------------------------------------------------------------*/
float vIn;
// VOLTAGE METER
const int analogInPin = 15;

//-----------------------------
// Definuje DHT11 pin and typ
#define DHTPIN 32 
#define DHTTYPE DHT11

float Konsanta_Vlhkost = 0;
float Konsanta_Teplota = 0;

// WiFi přihlašovací údaje
const char* ssid = "TP-LINK_DOLE"; //"Tp-ling" "DESKTOP-IF5CDUO" "TP-LINK_DOLE"
const char* password = "hromasova";   //"hromasova" "123456789"

// Web server port number
WiFiServer server(80);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

float tep, vlh, amp;

int Err;

//------- Slouží pro automatickou aktualizaci stránky --------
void refresh(WiFiClient& client){
  client.print("<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>");
  client.print("<p>This page will refresh automatically every 5 seconds.</p>");
  client.print("</body></html>");
  }

//------- Slouží pro vypis telot na web --------
void vypis_na_web(WiFiClient& client){
  if (Err == 1){
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(vIn) + "#" + String(amp) + "$" + String(Err) + "!" +"</span>";
    client.print(output);

    client.print("<h1>--- Rozvadec Baterie ---");
    client.print("</h3>");
    client.println("<h2>Failed to read from DHT sensor!</h3>"); 

    client.print("<h3>Napeti: ");
    client.print(vIn);
    client.print(" V</h5>");

    client.print("<h3>Proud: ");
    client.print(amp);
    client.print(" A</h5>");

    }else{
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(vIn) + "#" + String(amp) + "$" + String(Err) + "!" +"</span>";
    client.print(output);

    client.print("</h3>");

    client.print("<h1>--- Rozvadec Baterie ---");
    client.print("</h3>");

    client.print("<h2>Hodnoty ze senzoru: ");
    client.print("</h5>");
    
    client.print("<h3>Zarizeni: ");
    client.print(zarizeni);
    client.print("</h5>");

    client.print("<h3>Teplota: ");
    client.print(tep);
    client.print(" &deg;C</h5>");

    client.print("<h3>Vlhkost: ");
    client.print(vlh);
    client.print(" %</h5>");

    client.print("<h3>Napeti: ");
    client.print(vIn);
    client.print(" V</h5>");

    client.print("<h3>Proud: ");
    client.print(amp);
    client.print(" A</h5>");
    
   }
  }
  
// Connect to Wi-Fi network
void WiFi_Begin(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  IPAddress ip(192, 168, 0, 70); // fixní IP adresa           //192, 168, 137, 70
  IPAddress gateway(192, 168, 0, 1); // IP adresa brány       //192, 168, 137, 1
  IPAddress subnet(255, 255, 255, 0); // maska podsítě
  WiFi.config(ip, gateway, subnet); // nastavení fixní IP adresy

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void Data_Read(){
  vlh = dht.readHumidity() + (Konsanta_Vlhkost);
  tep = dht.readTemperature() + (Konsanta_Teplota);
            
  if (isnan(vlh)|| isnan(tep)) {
    Err = 1;            
  }else {
    Err = 0; 
  }

  amp = sensor.getCurrentDC();
  //ignoring the value below 0.09
  if (amp < 0.09) {
    amp = 0;
  }

  int sensorValue = analogRead(analogInPin); // načte analogovou hodnotu z pinu
  float voltage = sensorValue * (3.3 / 4095.0); // přepočítá hodnotu na napětí v rozsahu 0-3.3V
  Serial.println(voltage); // vypíše napětí na sériovou linku
  delay(500); // počká 1 sekundu

  /*voltageSensorVal = analogRead(voltageSensorPin);    // read the current sensor value (0 - 1023) 
  vOut = (voltageSensorVal / 1024) * vCC;             // convert the value to the real voltage on the analog pin
  vIn =  vOut * factor;

  Serial.println(voltageSensorVal);*/
  
}

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  pinMode(analogInPin, INPUT);
  analogReadResolution(12); // nastaví rozlišení převodu na 12 bitů

  // Initialize DHT sensor
  dht.begin();
  sensor.calibrate();
  // Connect to Wi-Fi network
  WiFi_Begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //----Slouží pro načtení hodnot a uložneí do proměnných---
            Data_Read();

            //------- Slouží pro vypis telot na web --------
            vypis_na_web(client);
            //------- Slouží pro automatickou aktualizaci stránky --------  
            //refresh(client);
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else {    // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }

}
