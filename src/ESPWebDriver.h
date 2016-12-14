#ifndef ESPWEBDRIVER_H
#define ESPWEBDRIVER_H

#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPFastLedDriver.h>
#include <math.h>

class ESPWebDriver
{
  public:
    ESPWebDriver(ESPFastLedDriver* ledDriver);
    void begin();
    void setRoutes();
    void LampStatusHandler(AsyncWebServerRequest *request);
    void LampPowerHandler(AsyncWebServerRequest *request);
    void LampColorHandler(AsyncWebServerRequest *request);
    void LampBrightnessHandler(AsyncWebServerRequest *request);
    void LampSpeedHandler(AsyncWebServerRequest *request);
    void LampPatternHandler(AsyncWebServerRequest *request);
    void LampPaletteHandler(AsyncWebServerRequest *request);
    void LampDelayHandler(AsyncWebServerRequest *request);
  private:
    ESPFastLedDriver* _ledDriver;
};

#endif
