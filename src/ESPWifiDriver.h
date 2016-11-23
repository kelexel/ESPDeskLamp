#ifndef ESPWifiDriver_H
#define ESPWifiDriver_H

#include <ESP8266WiFi.h>

class ESPWifiDriver
{
  public:
    ESPWifiDriver(const String hostname);
    void setupAP(const char* apWifiPSK);
    void setupClient(const char* ssid, const char* password);
    void setupClientNetwork(IPAddress ip, IPAddress netmask, IPAddress gateway);
    String formatHostname(const String hostname);
    void setupMDNS();
  private:
    bool _apMode;
    String _hostname;
};

#endif
