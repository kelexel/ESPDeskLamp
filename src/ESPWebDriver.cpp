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
}
void ESPWebDriver::setRoutes() {
  _server.on("/power", HTTP_GET, [this](AsyncWebServerRequest *request){
    int power = _ledDriver->getPower();
    request->send(200, "text/plain", String(power));
  });

  _server.on("/power", HTTP_POST, [this](AsyncWebServerRequest *request){
    AsyncWebParameter* power = request->getParam("value", true);
    _ledDriver->setPower(power->value().toInt());
    request->send(200, "text/plain", String("OK"));
  });

  _server.on("/brightness", HTTP_GET, [this](AsyncWebServerRequest *request){
    int brightness = _ledDriver->getBrightness();
    request->send(200, "text/plain", String(brightness));
  });

  _server.on("/brightness", HTTP_POST, [this](AsyncWebServerRequest *request){
    AsyncWebParameter* brightness = request->getParam("value", true);
    _ledDriver->setBrightness(brightness->value().toInt());
    request->send(200, "text/plain", String("OK"));
  });

}
void ESPWebDriver::handleRoot(AsyncWebServerRequest *request)
{
  String indexHTML = F("<!DOCTYPE html><html><head><title>File not found</title></head><body><h1>File not found.</h1></body></html>");
  Serial.println(ESP.getFreeHeap());
  request->send(200, "text/html", indexHTML);
}
