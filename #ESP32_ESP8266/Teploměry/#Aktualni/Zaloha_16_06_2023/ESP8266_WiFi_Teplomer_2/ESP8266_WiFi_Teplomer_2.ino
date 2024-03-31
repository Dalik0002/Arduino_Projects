#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//#include <ESP8266WebServer.h>

#define zarizeni 5

float Konsanta_Teplota = 5 ;

// Definuje pin senzoru
#define pin 5

// nastavení komunikace senzoru přes pin
OneWire oneWire(pin);     
// převedeme onewire do Dallasu                     
DallasTemperature sensors(&oneWire);

// WiFi přihlašovací údaje
const char* ssid = "Tp-ling"; //"Tp-ling" "DESKTOP-IF5CDU"
const char* password = "hromasova";   //"hromasova" "123456789"

// Web server port number
WiFiServer server(80);

float tep = 0;
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
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(Err) + "#" +"</span>";
    client.print(output);
    
    client.print("<h1>--- Pec na tuha paliva ---");
    client.print("</h3>");
    
    client.println("<h2>Chyna senzoru!</h3>"); 

    }else{
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(Err) + "#" +"</span>";
    client.print(output);

    client.print("</h3>");

    client.print("<h1>--- Pec na tuha paliva ---");
    client.print("</h3>");

    client.print("<h2>Hodnoty ze senzoru: ");
    client.print("</h5>");
    client.print("<h3>Zarizeni: ");
    client.print(zarizeni);
    client.print("</h5>");        
    client.print("<h3>Teplota: ");
    client.print(tep);
    client.print(" &deg;C</h5>");
   }
  }
  
// Connect to Wi-Fi network
void WiFi_Begin(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  IPAddress ip(192, 168, 0, 60); // fixní IP adresa       //192, 168, 137, 60
  IPAddress gateway(192, 168, 0, 1); // IP adresa brány   //192, 168, 137, 1
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


void setup() {
  // Initialize serial port
  Serial.begin(115200);
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

            sensors.requestTemperatures(); 
            tep = sensors.getTempCByIndex(0) + (Konsanta_Teplota) ;
            // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
            if (tep == -127) {
              // Chyba při měření
              Err = 1;
              Serial.println("Chyba při čtení z čidla!"); 
            } else {
              Err = 0;
              Serial.print("Teplota: "); 
              Serial.print(tep);
              Serial.print(" Stupnu Celsia, ");
            }
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