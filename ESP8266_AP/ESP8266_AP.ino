#include <ESP8266WiFi.h>

const char *ssid = "ESP8266";  // Název vašeho přístupového bodu
const char *password = "heslo123";   // Heslo pro přístupový bod

void setup() {
  Serial.begin(115200);
  delay(10);

  // Připojení k Wi-Fi síti jako přístupový bod
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("Přístupový bod: ");
  Serial.println(ssid);
  Serial.print("Heslo: ");
  Serial.println(password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP adresa přístupového bodu: ");
  Serial.println(myIP);
}

void loop() {
  // Vaši kód zde
}
