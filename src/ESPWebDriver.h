#ifndef ESPWEBDRIVER_H
#define ESPWEBDRIVER_H

#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPFastLedDriver.h>

class ESPWebDriver
{
  public:
    ESPWebDriver(ESPFastLedDriver* ledDriver);
    void begin();
    void setRoutes();
  private:
    ESPFastLedDriver* _ledDriver;
};

#endif
