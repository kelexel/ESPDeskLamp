#include <Arduino.h>
#include "ESPWebDriver.h"
// #include "ESPLedDriver.h"
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

AsyncWebServer _server(80);
// AsyncWebServer      web(HTTP_PORT);     /* Web Server */



ESPWebDriver::ESPWebDriver(ESPLedDriver* ledDriver) {
  _ledDriver = ledDriver;
}
void ESPWebDriver::begin() {
  _server.begin();
}
void ESPWebDriver::setRoutes() {

  _server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/json", _ledDriver->getStatus());
  });
  // Keep for compatibility with /js/scripts.js ... for now...
  _server.on("/all", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/json", _ledDriver->getStatus());
  });

  _server.on("/power", HTTP_GET, [this](AsyncWebServerRequest *request){
    Serial.print("Power: ");
    int power = _ledDriver->getPower();
    Serial.println(String(power));
    request->send(200, "text/plain", String(power));
  });

  _server.on("/power", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Power: ");
      String power = request->arg("value");
      Serial.println(String(power));
      _ledDriver->setPower(power.toInt());
      request->send(200, "text/plain", String(power));
    } else {
      Serial.print("Power: ");
      int power = _ledDriver->getPower();
      Serial.println(String(power));
      request->send(200, "text/plain", String(power));
    }
  });

  _server.on("/brightness", HTTP_GET, [this](AsyncWebServerRequest *request){
    Serial.print("Brightness: ");
    int brightness = _ledDriver->getBrightness();
    Serial.println(String(brightness));
    request->send(200, "text/plain", String(brightness));
  });

  _server.on("/brightness", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Brightness: ");
      String brightness = request->arg("value");
      Serial.println(String(brightness));
      _ledDriver->setBrightness(brightness.toInt());
      request->send(200, "text/plain", String(brightness));
    } else {
      Serial.print("Brightness: ");
      int brightness = _ledDriver->getBrightness();
      Serial.println(String(brightness));
      request->send(200, "text/plain", String(brightness));
    }
  });

  // _server.on("/speed", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Serial.print("Speed: ");
  //   String speed = String(_ledDriver->getSpeed());
  //   Serial.println(speed);
  //   request->send(200, "text/plain", speed);
  // });

  _server.on("/speed", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Speed: ");
      String speed = request->arg("value");
      Serial.println(speed);
      _ledDriver->setSpeed(speed.toInt());
      request->send(200, "text/plain", String(speed));
    } else {
      Serial.print("Speed: ");
      // String speed = String(_ledDriver->getSpeed());
      // Serial.println(speed);
      // request->send(200, "text/plain", speed);
    }
  });
  _server.on("/speed", HTTP_GET, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Speed: ");
      String speed = request->arg("value");
      Serial.println(speed);
      _ledDriver->setSpeed(speed.toInt());
      request->send(200, "text/plain", String(speed));
    } else {
      Serial.print("Speed: ");
      // String speed = String(_ledDriver->getSpeed());
      // Serial.println(speed);
      // request->send(200, "text/plain", speed);
    }
  });

  _server.on("/color", HTTP_GET, [this](AsyncWebServerRequest *request){
    Serial.print("Current color: ");
    String color = _ledDriver->getSolidColorHex();
    Serial.println(color);
    request->send(200, "text/plain", String(color));
    // _ledDriver->ack();
  });
  _server.on("/color", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Color: ");
      String color = request->arg("value");
      Serial.println(String(color));
      // Serial.print("RGB: ");
      struct CRGB rgbColor = _ledDriver->hexToRGB(color);
      // Serial.println(rgbColor);
      _ledDriver->setSolidColor(rgbColor);
      // Serial.print("HEX: ");
      // String hexColor = _ledDriver->RGBToHex(rgbColor);
      // Serial.println(hexColor);
      request->send(200, "text/plain", String(color));
    } else {
      Serial.print("Current color: ");
      String color = _ledDriver->getSolidColorHex();
      Serial.println(color);
      request->send(200, "text/plain", String(color));
    }
  });

  // _server.on("/pattern", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Serial.print("Pattern: ");
  //   String patternJson = _ledDriver->getCurrentPatternJson();
  //   Serial.println(patternJson);
  //   request->send(200, "text/json", patternJson);
  // });
  _server.on("/pattern", HTTP_GET, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      // Serial.print("Set Pattern: ");
      String pattern = request->arg("value");
      // Serial.println(String(pattern));
      _ledDriver->setPattern(pattern.toInt());
      request->send(200, "text/json", _ledDriver->getCurrentPatternJson());
    } else {
      request->send(200, "text/json", _ledDriver->getCurrentPatternJson());
    }
  });
  _server.on("/pattern", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Pattern: ");
      String pattern = request->arg("value");
      Serial.println(String(pattern));
      _ledDriver->setPattern(pattern.toInt());
      // int curPattern = _ledDriver->getCurrentPattern();
      request->send(200, "text/json", _ledDriver->getCurrentPatternJson());
    } else {
      // int curPattern = _ledDriver->getCurrentPattern();
      request->send(200, "text/json", _ledDriver->getCurrentPatternJson());
    }
  });

  _server.on("/palette", HTTP_GET, [this](AsyncWebServerRequest *request){
    Serial.print("Palette: ");
    String palette = String(_ledDriver->getCurrentPalette());
    Serial.println(palette);
    request->send(200, "text/plain", palette);
  });

  _server.on("/palette", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Set Palette: ");
      String palette = String(request->arg("value"));
      Serial.println(palette);
      _ledDriver->setPalette(palette.toInt());
      request->send(200, "text/plain", palette);
    } else {
      request->send(200, "text/plain", String(_ledDriver->getCurrentPalette()));
    }
  });

  _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

  _server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
}
