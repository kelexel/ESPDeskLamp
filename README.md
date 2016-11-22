FastLED + ESP8266 Async Web Server
=========

Control an addressable LED strip with an ESP8266 via a web browser or infrared remote control, or MQTT, or homekit.

* Built with Platformio
* Works with *homebridge-better-http-rgb* (yay!), see below.

Purpose
-------

This is a humble rewrite attempt of FastLED + ESP8266 Web Server by Jason Coon: https://github.com/jasoncoon/esp8266-fastled-webserver

It also includes ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb


The original code relied on the regular WebServer package for the ESP, which as you probably know, doesn't play really well if you query it too frequently...

The idea is to use ESPAsyncWebServer instead, which as the name implies, runs asynchronously and, in general, works pretty well.

Features
--------

Currently, none that make this usable.
* *ESPWifiDriver* is a class made to handle the basic network stack: act as an access point, or act as a wifi client, advertise the services using mDNS (except mDNS is broken for now)
* *ESPLedDriver* is a class made to handle the FastLED settings, animations, and palettes.
* *ESPWebDriver* is a class made to handle HTTP requests, and invoke the ESPLedDriver accordingly.
* *ESPIRDriver* is a class made to handle IR remote codes, and invoke the ESPLedDriver accordingly.


I plan to include the following:
* IR support from the original "FastLED + ESP8266 Web Server"
* MQTT support
* HTTP Ui support, either from the original "FastLED + ESP8266 Web Server" or a home baked one.


Status
------

* ESPLedDriver::hexToRGB and ESPLedDriver::RGBToHex probably need a rewrite to improve conversion speed.
* It crashes from time to time, not sure -yet- why.
* MDNS isn't working, I need to dig why, but I hope *ESPWifiDriver* kind of works
* The *ESPIRDriver* is roughly a class, for now.
* This is my first attempt at writing C++ code, so expect to find many mistakes... Please, feel free to fork and make a PR...


Install
-------

Download Platformio from http://platformio.org

Note if you are using Atom already, I recommend you get their Atom based IDE..

Just know that it will break havoc on your regular Atom, and enable a (barely functional) platformio+platformio_cli+build plugins..

For some reason launching their Atom form works better.

Download the ESP8266 platform for platformio when asked to install a board

Install the following libraries using `pio lib update`
* ESPAsyncTCP
* ESPAsyncWebServer
* FastLED
* IRremoteESP8266
* esp8266_mdns

Than build and flash


homebridge-better-http-rgb setup
--------------------------------

Install homebridge following directions found at https://github.com/nfarina/homebridge

Install homebridge-better-http-rgb using `npm install -g homebridge-better-http-rgb`

In your `.homebridge/config.conf` file, add something like this (replacing 192.168.6.243 with your device's IP address):

```
"accessories": [
    {
      "accessory": "HTTP-RGB",
      "name": "Single Color Light",
      "http_method": "POST",
      "switch": {
        "status": "http://192.168.6.243/power",
        "powerOn": "http://192.168.6.243/power?value=1",
        "powerOff": "http://192.168.6.243/power?value=0"
      },
      "brightness": {
        "status": "http://192.168.6.243/brightness",
        "url": "http://192.168.6.243/brightness?value=%s"
      },
      "color": {
        "status": "http://192.168.6.243/color",
        "url": "http://192.168.6.243/color?value=%s",
        "brightness": false
      }
    }
  ]
```

Hardware
--------

An ESP8266 development board, such as the [Adafruit HUZZAH ESP8266 Breakout], but smaller boards like the ESP-01 are supported.

[![Adafruit HUZZAH ESP8266 Breakout](https://cdn-shop.adafruit.com/310x233/2471-10.jpg)](https://www.adafruit.com/products/2471)

Addressable LED strip, such as the [Adafruit NeoPixel Ring], or any FastLED supported strip:

[![Adafruit NeoPixel Ring](https://www.adafruit.com/images/145x109/1586-00.jpg)](https://www.adafruit.com/product/1586)
