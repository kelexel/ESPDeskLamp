#ifndef ESPWEBDRIVER_H
#define ESPWEBDRIVER_H

#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPLedDriver.h>

class ESPWebDriver
{
  public:
    ESPWebDriver(ESPLedDriver* ledDriver);
    void begin();
    void setRoutes();
  private:
    ESPLedDriver* _ledDriver;
};

#endif
