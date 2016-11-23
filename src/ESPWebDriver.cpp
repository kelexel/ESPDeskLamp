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
  // _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   request->send(200, "text/plain", String("Hello world!"));
  // });

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
      Serial.print("Power: ");
      String power = request->arg("value");
      Serial.println(String(power));
      _ledDriver->setPower(power.toInt());
      request->send(200, "text/plain", String(power));
    } else {
      request->send(200, "text/plain", String("Error"));
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
      Serial.print("Brightness: ");
      String brightness = request->arg("value");
      Serial.println(String(brightness));
      _ledDriver->setBrightness(brightness.toInt());
      request->send(200, "text/plain", String(brightness));
    } else {
      Serial.println("Invalid arg received for /brightness");
      request->send(200, "text/plain", String("Error"));
    }
  });

  _server.on("/color", HTTP_GET, [this](AsyncWebServerRequest *request){
    Serial.print("Current color: ");
    String color = _ledDriver->getSolidColorHex();
    Serial.println(color);
    request->send(200, "text/plain", String(color));
  });
  _server.on("/color", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Color: ");
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
      Serial.println("Invalid arg received for /color");
      request->send(200, "text/plain", String("Error"));
    }
  });

  _server.on("/pattern", HTTP_POST, [this](AsyncWebServerRequest *request){
    if(request->hasArg("value")) {
      Serial.print("Pattern: ");
      String pattern = request->arg("value");
      Serial.println(String(pattern));
      _ledDriver->setPattern(pattern.toInt());
      request->send(200, "text/plain", String(pattern));
    } else {
      Serial.println("Invalid arg received for /pattern");
      request->send(200, "text/plain", String("Error"));
    }
  });


  _server.serveStatic("/index.htm", SPIFFS, "/index.htm");
  _server.serveStatic("/fonts", SPIFFS, "/fonts", "max-age=86400");
  _server.serveStatic("/js", SPIFFS, "/js");
  _server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
  _server.serveStatic("/images", SPIFFS, "/images", "max-age=86400");
  _server.serveStatic("/", SPIFFS, "/index.htm");

  _server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
}

void ESPWebDriver::handleRoot(AsyncWebServerRequest *request)
{
  String indexHTML = F("<!DOCTYPE html><html><head><title>File not found</title></head><body><h1>File not found.</h1></body></html>");
  Serial.println(ESP.getFreeHeap());
  request->send(200, "text/html", indexHTML);
}
