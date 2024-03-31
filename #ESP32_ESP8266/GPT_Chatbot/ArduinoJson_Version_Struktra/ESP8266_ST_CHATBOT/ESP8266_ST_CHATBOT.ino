#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

const char* ssid = "MyESP32AP"; // název Access Pointu
const char* password = "password123"; // heslo pro Access Point

const char* server_IP = "192.168.4.1"; // adresa ESP32 Access Pointu
const int port = 80;

#define pinDHT 2
#define typDHT11 DHT11
DHT dht(pinDHT, typDHT11); // inicializace teplotního senzoru DHT22

struct DhtData {
  float tep;
  float vlh;
};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  dht.begin();
}

void loop() {
  float tep = dht.readTemperature()-4; // načtení teploty z DHT22
  float vlh = dht.readHumidity(); // načtení teploty z DHT22
  if (isnan(tep) || isnan(vlh)) {
    Serial.println("Failed to read temperature from DHT sensor");
    delay(1000);
  }else{
  Serial.print("Teplota: ");
  Serial.println(tep);
  Serial.print("Vlhkost: ");
  Serial.println(vlh);

StaticJsonDocument<200> doc; // vytvoření JsonDocument objektu s kapacitou 200 bajtů
    doc["teplota"] = tep;
    doc["vlhkost"] = vlh;
    String json;
    serializeJson(doc, json);

  // establish Wi-Fi connection to ESP32
  WiFiClient client;
  if (client.connect(server_IP, port)) {
    Serial.println("Connected to server");
    client.println(json);
    client.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }

  delay(1000);
  }
}
