#include <Arduino.h>
#include "ESPWifiDriver.h"
#include <ESP8266mDNS.h>

ESPWifiDriver::ESPWifiDriver(const String hostname) {
  _hostname = formatHostname(hostname);
  // Do something clever here ?
}

String ESPWifiDriver::formatHostname(const String hostname) {
  // From: https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-goodnight-thing-sleep-mode
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  // String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                //  String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return hostname + macID;
}
void ESPWifiDriver::setupAP(const char* apWifiPSK){
  WiFi.hostname(_hostname);
  WiFi.mode(WIFI_AP);

  String AP_NameString = _hostname;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, apWifiPSK);
  WiFi.begin();
}

void ESPWifiDriver::setupClientNetwork(IPAddress ip, IPAddress g, IPAddress n) {
  Serial.println("Client DHCP: OFF ");
  Serial.printf("IP Address: %u.%u.%u.%u \n", ip[0], ip[1], ip[2], ip[3]);
  Serial.printf("Gateway Address: %u.%u.%u.%u \n", g[0], g[1], g[2], g[3]);
  Serial.printf("Netmask Address: %u.%u.%u.%u \n", n[0], n[1], n[2], n[3]);

  WiFi.config(ip, g, n);
}
void ESPWifiDriver::setupClient(const char* ssid, const char* password) {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.hostname(_hostname);

  // if (String(WiFi.SSID()) != String(ssid)) {
    // Serial.println("A");
 //   WiFi.begin(ssid, password);
  // }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.print(" or http://");
  Serial.print(_hostname);
  Serial.println(".local in your browser");

}
void ESPWifiDriver::setupMDNS() {
  Serial.println(String(_hostname));
  char host[sizeof(_hostname)];
  _hostname.toCharArray(host, sizeof(host));
  Serial.println(String(host));

  Serial.println();
  if (!MDNS.begin(host)) {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(500);
    }
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("MDNS responder started");
}
