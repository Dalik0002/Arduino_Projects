#include <ArduinoJson.h>
#include <WiFi.h>

const char* ssid = "MyESP32AP"; // název Access Pointu
const char* password = "password123"; // heslo pro Access Point

WiFiServer server(80);

struct DhtData {
  float tep;
  float vlh;
};

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);
  delay(100);
  server.begin();
  Serial.println("Access Point started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();

        // deserialize received data into JsonObject
        StaticJsonDocument<200> doc;
        deserializeJson(doc, request);
        JsonObject obj = doc.as<JsonObject>();

        // get temperature and humidity values
        float tep = obj["tep"];
        float vlh = obj["vlh"];

        // print received data
        Serial.print("Teplota: ");
        Serial.println(tep);
        Serial.print("Vlhkost: ");
        Serial.println(vlh);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(100);
}
