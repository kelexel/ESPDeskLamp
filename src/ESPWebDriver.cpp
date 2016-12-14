#include <Arduino.h>
#include "ESPWebDriver.h"
// #include "ESPFastLedDriver.h"
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

AsyncWebServer _server(80);
// AsyncWebServer      web(HTTP_PORT);     /* Web Server */



ESPWebDriver::ESPWebDriver(ESPFastLedDriver* ledDriver) {
  _ledDriver = ledDriver;
}
void ESPWebDriver::begin() {
  _server.begin();
}

void ESPWebDriver::LampStatusHandler(AsyncWebServerRequest *request) {
  request->send(200, "text/json", _ledDriver->getStatus());
}

void ESPWebDriver::LampPowerHandler(AsyncWebServerRequest *request) {
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

}
void ESPWebDriver::LampColorHandler(AsyncWebServerRequest *request){
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
}
void ESPWebDriver::LampPatternHandler(AsyncWebServerRequest *request){
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
}
void ESPWebDriver::LampPaletteHandler(AsyncWebServerRequest *request){
  if(request->hasArg("value")) {
    Serial.print("Set Palette: ");
    String palette = String(request->arg("value"));
    Serial.println(palette);
    _ledDriver->setPalette(palette.toInt());
    request->send(200, "text/plain", palette);
  } else {
    request->send(200, "text/plain", String(_ledDriver->getCurrentPalette()));
  }
}
void ESPWebDriver::LampBrightnessHandler(AsyncWebServerRequest *request){
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
}
void ESPWebDriver::LampSpeedHandler(AsyncWebServerRequest *request){
  if(request->hasArg("type") && request->hasArg("value")) {
    Serial.print("Set Speed: ");
    String speedType = request->arg("type");
    String speed = request->arg("value");
    _ledDriver->setSpeed(speedType, speed.toInt());
    String json = "{"+speedType+": {value: \""+speed+"\", translated: \""+_ledDriver->getSpeed(speedType)+"\"}}";
    Serial.println(json);
    request->send(200, "text/json", json);
  } else {
    Serial.print("Speed: Error");
    // String speed = String(_ledDriver->getSpeed());
    // Serial.println(speed);
    // request->send(200, "text/plain", speed);
  }
}

// void ESPWebDriver::LampDelayHandler(AsyncWebServerRequest *request){
//   if(request->hasArg("value") && request->hasArg("type")) {
//     Serial.print("Set Type: ");
//     String type = request->arg("type");
//     Serial.print(type);
//     Serial.print(" value: ");
//     String speed = request->arg("value");
//     Serial.println(speed);
//     _ledDriver->setDelay(type.toInt(), speed.toInt());
//     request->send(200, "text/plain", String(speed));
//   } else {
//     Serial.print("Speed: ");
//     // String speed = String(_ledDriver->getSpeed());
//     // Serial.println(speed);
//     // request->send(200, "text/plain", speed);
//   }
// }

void ESPWebDriver::setRoutes() {

  _server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampStatusHandler(request); });
  _server.on("/all", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampStatusHandler(request); });

  _server.on("/power", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampPowerHandler(request); });
  _server.on("/power", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampPowerHandler(request); });

  _server.on("/brightness", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampBrightnessHandler(request); });
  _server.on("/brightness", HTTP_POST, [this](AsyncWebServerRequest *request){ LampBrightnessHandler(request); });


  // _server.on("/setDelay", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampDelayHandler(request); });
  // _server.on("/setDelay", HTTP_POST, [this](AsyncWebServerRequest *request){ LampDelayHandler(request); });


  _server.on("/speed", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampSpeedHandler(request); });
  _server.on("/speed", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampSpeedHandler(request); });

  _server.on("/pattern", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampPatternHandler(request); });
  _server.on("/pattern", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampPatternHandler(request); });

  _server.on("/palette", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampPaletteHandler(request); });
  _server.on("/palette", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampPaletteHandler(request); });

  _server.on("/color", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampColorHandler(request); });
  _server.on("/color", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampColorHandler(request); });

  _server.on("/solidColor", HTTP_GET, [this](AsyncWebServerRequest *request){ this->LampColorHandler(request); });
  _server.on("/solidColor", HTTP_POST, [this](AsyncWebServerRequest *request){ this->LampColorHandler(request); });

  _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

  _server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
}
