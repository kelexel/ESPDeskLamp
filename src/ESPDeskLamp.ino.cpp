# 1 "/var/folders/f1/732kh2qd06g93jv6m6drn58h0000gn/T/tmpXisTcu"
#include <Arduino.h>
# 1 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
#include <Arduino.h>
#include <EEPROM.h>
extern "C" {
#include "user_interface.h"
}
# 29 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
#include <IRremoteESP8266.h>
#include "ESPWifiDriver.h"
#include "ESPFastLedDriver.h"
#include "ESPWebDriver.h"
# 45 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
const String hostname = "DeskLamp";






const bool apMode = true;

const char apWifiPSK[] = "HelloHowAreYu";





const char* ssid = "neuro";
const char* password = "HelloHowAreYu";

const bool clientUseDHCP = true;


IPAddress ip(192, 168, 6, 243);
IPAddress gateway(192, 168, 6, 253);
IPAddress netmask(255, 255, 255, 0);
# 85 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
ESPWifiDriver wifidriver(hostname);
ESPFastLedDriver leddriver;
ESPWebDriver webdriver(&leddriver);
# 96 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
void setup();
void loop();
#line 96 "/Users/rud/Desktop/Dev/lamp/ESPDeskLamp/src/ESPDeskLamp.ino"
void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);

  Serial.println();
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.println();

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }


  if (apMode) {
    wifidriver.setupAP(apWifiPSK);
  } else {
    if (!clientUseDHCP) {
      wifidriver.setupClientNetwork(ip, gateway, netmask);
    }
    wifidriver.setupClient(ssid, password);
  }

  leddriver.setup();
  webdriver.setRoutes();
  webdriver.begin();
  Serial.print(leddriver.getStatus());
}

void loop() {
  leddriver.run(30);
}