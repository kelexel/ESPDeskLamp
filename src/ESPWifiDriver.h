#ifndef ESPWifiDriver_H
#define ESPWifiDriver_H

#include <ESP8266WiFi.h>

class ESPWifiDriver
{
  public:
    ESPWifiDriver();
    void setupAP(const String hostname, const char* apWifiPSK);
    void setupClient(const String hostname, const char* ssid, const char* password);
    void setupClientNetwork(IPAddress ip, IPAddress netmask, IPAddress gateway);
    void setupMDNS(const String hostname);
  private:
    bool _apMode;
};

#endif
