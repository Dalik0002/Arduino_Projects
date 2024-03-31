// Import required libraries
#include <ESP8266WiFi.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "DESKTOP-IF5CDUO";
const char* password = "123456789";

// Web server port number
WiFiServer server(80);

// DHT sensor pin
#define DHTPIN 2 // GPIO5 (D1)

// DHT sensor type
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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


            // Read values from DHT sensor
            float h = dht.readHumidity();
            float t = dht.readTemperature();

            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t)) {
              client.println("<h3>Failed to read from DHT sensor!</h3>");
            }
            else {
              // Display values on web page
              client.print("<h3>Temperature: ");
              client.print(t);
              client.print(" &deg;C</h3>");
              client.print("<h3>Humidity: ");
              client.print(h);
              client.print(" %</h3>"); 
              }

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
