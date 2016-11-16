#include <Arduino.h>
#include "ESPWifiDriver.h"
#include <ESP8266mDNS.h>

ESPWifiDriver::ESPWifiDriver() {
  // Do something clever here ?
}

void ESPWifiDriver::setupAP(const String hostname, const char* apWifiPSK){
  WiFi.hostname(hostname);
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = hostname + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, apWifiPSK);
  WiFi.begin();
}

void ESPWifiDriver::setupClientNetwork(IPAddress ip, IPAddress g, IPAddress n) {
  Serial.print("Client DHCP: OFF");
  Serial.printf("IP Address: %u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  Serial.printf("Gateway Address: %u.%u.%u.%u", g[0], g[1], g[2], g[3]);
  Serial.printf("Netmask Address: %u.%u.%u.%u", n[0], n[1], n[2], n[3]);

  WiFi.config(ip, g, n);
}
void ESPWifiDriver::setupClient(const String hostname, const char* ssid, const char* password) {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.hostname(hostname);

  // if (String(WiFi.SSID()) != String(ssid)) {
    // Serial.println("A");
 //   WiFi.begin(ssid, password);
  // }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");

}
void ESPWifiDriver::setupMDNS(const String hostname) {
  // Uncommenting the following produces a bug : src/ESPWifiDriver.cpp:62:20: error: no matching function for call to 'MDNSResponder::begin(const String&)'

  // if (!MDNS.begin(hostname)) {
  //   Serial.println("Error setting up MDNS responder!");
  //   while(1) {
  //     delay(1000);
  //   }
  // }
  Serial.println("mDNS responder started");
}
