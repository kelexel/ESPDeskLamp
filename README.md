ESP Desk Lamp
=============

An ESP8266 and FastLED desk *animated* lamp with support for HTTP and Homekit (via) Homebridge



About
-----
This is a humble rewrite of [[FastLED + ESP8266 Web Server](https://github.com/jasoncoon/esp8266-fastled-webserver)] by *Jason Coon*, using [[ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)] and lots of Class refactoring.

The original code relied on the regular WebServer package for the ESP, which as you probably know, doesn't play really well if you query it too frequently...

The idea is to use [[ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)] instead, which as the name implies, runs asynchronously and, in general, works pretty well.

It also includes [[ColorWavesWithPalettes]( https://gist.github.com/kriegsman/8281905786e8b2632aeb)] by *Mark Kriegsman*

And bits of codes from many sources I (try to) reference in the code.

This is my first attempt at writing C++ code, so expect to find many mistakes... Incoherences... Stupidity...

Please, feel free to fork this project, make a PR, or open a ticket in the Issues section.

*11/23/2016* - Rudolph Sand (Kelexel)


Features
--------
* Control one (or more ?) addressable LED strip(s) with an ESP8266 compatible board.
* Uses [[FastLED](http://fastled.io/)]
* Works with [[homebridge-better-http-rgb]()], so yes, this means *Siri & Homekit support*.
* HTTP UI, borrowed, and adapted from the original [[FastLED + ESP8266 Web Server](https://github.com/jasoncoon/esp8266-fastled-webserver)] project, for now. I need to add debounce to click events, possibly use a more mobile centric JS/CSS FW, and of course, websockets <o/
* 10 animations from the [[FastLED DemoReel](https://github.com/FastLED/FastLED/tree/master/examples/DemoReel100)].
* Built with [[Platformio](http://platformio.org)]: use Platformio to build and flash the esp8266
* Set Power, Brightness, Speed, Hue, Palette, Pattern via an API running on [[ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)] (async! yay!)


A crash course into C++, and Classes
-------------------------------
* Again, again, again, this is my first attempt at some C++ code. Some methods will probably be refactored.
* This is a training exercise for me. Go from A to B, using the most elegant *(hum...)* route.
* *ESPWifiDriver* is a class made to handle the basic network stack: act as an access point, or act as a wifi client, advertise the services using mDNS. It might become Singleton...
* *ESPFastLedDriver* is a class made to handle the FastLED settings, animations, and palettes.
* *ESPWebDriver* is a class made to handle HTTP requests, and invoke the ESPFastLedDriver accordingly. It might also become a Singleton
* *ESPIRDriver* is garbage for now, and will probably remain that way until I get a proper IR Receiver module


Changelog:
----------

* v1.2
  * Found the reason for random crashes, and fixed it
  * Modified the html UI to fit my needs (it will do for now)
  * Tested on ESP-01 and NodeMCU v3 boards

* v1.1
  * Added animations support
  * Still randomly crashes

* v1.0
  * Initial release, contains ESPWifiDriver, ESPLedDriver, but no animations

Todo:
-----
* A massive code cleanup. This should happen soon.
* Make ESP*PixelBus*Driver Class (probably using an ESPLedDriver abstract class), and let you chose which you prefer ;)
* MQTT support via [[async-mqtt-client](https://github.com/marvinroger/async-mqtt-client)]
* IR support like, the original "FastLED + ESP8266 Web Server" project
* ESPFastLedDriver::hexToRGB and ESPFastLedDriver::RGBToHex probably need a rewrite to improve conversion speed.
* It crashes from time to time, not sure -yet- why.. #UPDATE: It doesn't crash anymore !
* *ESPWifiDriver* works well (I think ?)
* The *ESPIRDriver* is roughly a class, for now.


Install
-------
Download Platformio from [[here](http://platformio.org)]

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

You might want to edit ESPLedDriver.h to specify which PIN is hooked to the LEDs (see `DATA_PIN`).
You might also want to edit the number of LEDs in the same file (see `NUM_LEDS`)

Than build and flash.


Flash SPIFFS to esp8266:
------------------------
Open a terminal in Platformio IDE.

Type: `platformio run --target buildfs` to build the filesystem image.

Type `platformio run --target uploadfs` to upload the image to the esp8266.

Note:
I currently added the ESP-01 1M/512K flags in platformio.ini, aswell as the NodeMCU 4M/1M flags.
You might want to adjust to your board, see Platformio docs:
[[uploading-files-to-file-system-spiffs](http://docs.platformio.org/en/stable/platforms/espressif8266.html#uploading-files-to-file-system-spiffs)] and [[platform-espressif-customflash](http://docs.platformio.org/en/stable/platforms/espressif8266.html#platform-espressif-customflash)]


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

References
----------

* https://github.com/jasoncoon/esp8266-fastled-webserver
* http://internetofhomethings.com/homethings/?p=396
* http://docs.platformio.org/en/stable/platforms/espressif8266.html#uploading-files-to-file-system-spiffs
* http://docs.platformio.org/en/stable/platforms/espressif8266.html#platform-espressif-customflash
* https://github.com/me-no-dev/ESPAsyncWebServer
