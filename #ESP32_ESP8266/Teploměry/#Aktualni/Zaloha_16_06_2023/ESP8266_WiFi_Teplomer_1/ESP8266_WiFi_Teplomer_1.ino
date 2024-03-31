#include <ESP8266WiFi.h>
#include <DHT.h>
//#include <ESP8266WebServer.h>

#define zarizeni 4

// Definuje DHT11 pin and typ
#define DHTPIN 2 
#define DHTTYPE DHT11

// WiFi přihlašovací údaje
const char* ssid = "Tp-ling"; //"Tp-ling"
const char* password = "hromasova";   //"hromasova"

// Web server port number
WiFiServer server(80);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

float tep, vlh;

int Err1;

float Konsanta_Vlhkost = -14;
float Konsanta_Teplota = -5;


//------- Slouží pro automatickou aktualizaci stránky --------
void refresh(WiFiClient& client){
  client.print("<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>");
  client.print("<p>This page will refresh automatically every 5 seconds.</p>");
  client.print("</body></html>");
  }

//------- Slouží pro vypis telot na web --------
void vypis_na_web(WiFiClient& client){
  if (Err1 == 1){
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(vlh) + "#" + String(Err1) + "!" +"</span>";
    client.print(output);

    client.print("<h1>--- Spajska ---");
    client.print("</h3>");
    
    client.println("<h2>Failed to read from DHT sensor!</h3>"); 

    }else{
    String output = String("<span style='display:none'>") + "?" + String(zarizeni) + "%" + String(tep) + "&" + String(vlh) + "#" + String(Err1) + "!" +"</span>";
    client.print(output);

    client.print("</h3>");

    client.print("<h1>--- Spajska ---");
    client.print("</h3>");

    client.print("<h2>Hodnoty ze senzoru: ");
    client.print("</h3>");
    client.print("<h3>Zarizeni: ");
    client.print(zarizeni);
    client.print("</h5>");        
    client.print("<h3>Teplota: ");
    client.print(tep);
    client.print(" &deg;C</h5>");
    client.print("<h3>Vlhkost: ");
    client.print(vlh);
    client.print(" %</h5>");
   }
  }
  
// Connect to Wi-Fi network
void WiFi_Begin(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  IPAddress ip(192, 168, 0, 50); // fixní IP adresa       //192, 168, 137, 50
  IPAddress gateway(192, 168, 0, 1); // IP adresa brány   // 192, 168, 137, 1
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
  // Initialize DHT sensor
  dht.begin();
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

            vlh = dht.readHumidity() + (Konsanta_Vlhkost);
            tep = dht.readTemperature() + (Konsanta_Teplota);
            
            if (isnan(vlh)|| isnan(tep)) {
              Err1 = 1;            
            }else {
              Err1 = 0; }
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
