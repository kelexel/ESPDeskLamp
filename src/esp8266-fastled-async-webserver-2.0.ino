#include <Arduino.h>
#include <EEPROM.h>

/*
* esp8266-fastled-async-webserver
* A humble attempt at porting esp8266-fastled-webserver to ESPAsyncWebServer + MQTT support
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
#include "ESPLedDriver.h"
#include "ESPWebDriver.h"
#include "ESPIRDriver.h"
#include "Animation.h"



/*
Config Start
*/

// Hostname for the Lmp
const String hostname = "Lmp";


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

const bool clientUseDHCP = false;

// ip, gateway, netmask, only used if ClientUseDHCP = true
IPAddress ip(192, 168, 6, 243);
IPAddress gateway(192, 168, 6, 253);
IPAddress netmask(255, 255, 255, 0);


/*
IR Settings
*/
#define RECV_PIN 12


/*
Config End
*/









// Instantiate the "drivers"
IRrecv irReceiver(RECV_PIN);

ESPWifiDriver wifidriver;
ESPLedDriver pixeldriver;
ESPWebDriver webdriver(&pixeldriver);
ESPIRDriver irdriver(&irReceiver, &pixeldriver);

void setup() {
  // Start the serial connection
  Serial.begin(115200);
  EEPROM.begin(512);

  // Start the wifi driver in either AP or Client mode
  if (apMode) {
    wifidriver.setupAP(hostname, apWifiPSK);
  } else {
    if (!clientUseDHCP) {
      wifidriver.setupClientNetwork(ip, gateway, netmask);
    }
    wifidriver.setupClient(hostname, ssid, password);
  }
  wifidriver.setupMDNS(hostname);
}

void loop() {

}
