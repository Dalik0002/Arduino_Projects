#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Nastavení WiFi
const char* ssid = "FKH455";
const char* password = "HGfk25462";

// Nastavení MQTT
const char* mqtt_server = "test.mosquitto.org"; // Zde vložte adresu vašeho MQTT brokera
WiFiClient espClient;
PubSubClient client(espClient);

// Nastavení DHT22
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}

void setup_wifi() {
  delay(10);
  // Připojení k WiFi síti
  Serial.println();
  Serial.print("Pripojovani k ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi pripojeno");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Čtení teploty a vlhkosti
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Kontrola, zda se čtení povedlo
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Selhalo cteni z DHT senzoru!");
    return;
  }

  // Vytvoření JSON objektu
  String jsonData = "{\"temperature\": " + String(temperature) + ",\"humidity\": " + String(humidity) + "}";

  // Publikování na MQTT
  client.publish("Domena/dht11", jsonData.c_str());
  delay(5000);
}

void reconnect() {
  // Opětovné připojení k MQTT
  while (!client.connected()) {
    Serial.print("Pripojovani k MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("pripojeno");
    } else {
      Serial.print("selhalo, rc=");
      Serial.print(client.state());
      Serial.println(" zkousim za 5 sekund znovu");
      delay(5000);
    }
  }
}
