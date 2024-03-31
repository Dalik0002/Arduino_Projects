#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Definuje DHT11 pin and typ
#define DHTPIN 2 
#define DHTTYPE DHT11

// WiFi přihlašovací údaje
const char* ssid = "FKH455"; //FKH455 ..... DESKTOP-IF5CDUO
const char* password = "HGfk25462"; //HGfk25462 .... 135792468

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
//const char* mqtt_user = "UŽIVATEL_BROKERU_MQTT";
//const char* mqtt_password = "HESLO_UŽIVATELE_BROKERU_MQTT";
const char* mqtt_topic = "CoreElectronics/VSB_JSON";

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

float tep = 0, vlh = 0, id = 0;
bool Err = true;


WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(){

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

}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Připojování k MQTT brokeru...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Připojeno k MQTT brokeru");
    } else {
      Serial.println("Nepodařilo se připojit k MQTT brokeru, zkouším znovu za 3 sekundu...");
      delay(3000);
    }
  }
}

void setup() {
  // Initialize serial port
  Serial.begin(115200);
  // Initialize DHT sensor
  dht.begin();
  // Connect to Wi-Fi network
  setup_wifi();
  // MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  
  id++;

  float vlh = dht.readHumidity();
  float tep = dht.readTemperature();
  unsigned long myTime = millis()/1000;
  
  if (isnan(vlh) || isnan(tep)) {
    Err = true;
    vlh = 0;
    tep = 0;
  } else {
    Err = false;
  }
  
  // Vytvořte JSON objekt
  StaticJsonDocument<200> doc;
  doc["id"] = id;
  doc["time"] = myTime;
  doc["error"] = Err;
  doc["temperature"] = tep;
  doc["humidity"] = vlh;
  
  // Převedení JSON na řetězec
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Odeslání JSONu na MQTT topic
  client.publish(mqtt_topic, jsonString.c_str());
  
  delay(5000); // Odeslat JSON každých 5 sekund

  Serial.print("Odesilana hodnota: ");
  Serial.println(jsonString);
}
