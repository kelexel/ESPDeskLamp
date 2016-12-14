#include <Arduino.h>
#include <EEPROM.h>
extern "C" {
#include "user_interface.h"
}

/*
* ESPDeskLamp (aka esp8266-fastled-async-webserver)
* A humble attempt at porting esp8266-fastled-webserver to ESPAsyncWebServer + MQTT support
* Copyright (c) 2016 Rudolph Sand (kelexel@github)
*
* Based upon:
* ESP8266 + FastLED + IR Remote + MSGEQ7: https://github.com/jasoncoon/esp8266-fastled-webserver
* Copyright (C) 2015 Jason Coon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <IRremoteESP8266.h>
#include "ESPWifiDriver.h"
#include "ESPFastLedDriver.h"
#include "ESPWebDriver.h"



/*
Config Start
*/

// Hostname for the Lmp
const String hostname = "DeskLamp";


/*
Wireless access point settings
*/
// Operate as a standalone AccessPoint ? (set false for wifi client)
const bool apMode = false;
// Set a PreSharedKey to connect to the AP ? (leave blank for no PSK)
const char apWifiPSK[] = "";


/*
Wireless client settings (apMode = false)
*/
const char* ssid = "neuro";
const char* password = "HelloHowAreYu";

const bool clientUseDHCP = true;

// ip, gateway, netmask, only used if ClientUseDHCP = true
IPAddress ip(192, 168, 6, 243);
IPAddress gateway(192, 168, 6, 253);
IPAddress netmask(255, 255, 255, 0);




/*
Config End
*/









ESPWifiDriver wifidriver(hostname);
ESPFastLedDriver leddriver;
ESPWebDriver webdriver(&leddriver);

/* IR Driver not tested */
// #include "ESPIRDriver.h"
// #define RECV_PIN 12
// IRrecv irReceiver(RECV_PIN);
// ESPIRDriver irdriver(&irReceiver, &leddriver);


void setup() {
  // Start the serial connection
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

  // Start the wifi driver in either AP or Client mode
  if (apMode) {
    wifidriver.setupAP(apWifiPSK);
  } else {
    if (!clientUseDHCP) {
      wifidriver.setupClientNetwork(ip, gateway, netmask);
    }
    wifidriver.setupClient(ssid, password);
  }
  wifidriver.setupMDNS();
  leddriver.setup();
  webdriver.setRoutes();
  webdriver.begin();
  Serial.print(leddriver.getStatus());
}

void loop() {
  leddriver.run(30);
}
