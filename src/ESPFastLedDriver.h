
#ifndef ESPLEDRIVER_H
#define ESPLEDRIVER_H

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>


FASTLED_USING_NAMESPACE


  #define DATA_PIN      2     // For ESP-01
  // #define DATA_PIN      7     // for Huzzah: Pins w/o special function:  #4, #5, #12, #13, #14; // #16 does not work :(
  #define LED_TYPE      WS2811
  #define COLOR_ORDER   GRB
  // #define COLOR_ORDER RGB

  #define NUM_LEDS      9

  #define MILLI_AMPS         2000     // IMPORTANT: set here the max milli-Amps of your power supply 5V 2A = 2000
  //#define FRAMES_PER_SECOND  speed // here you can control the speed. With the Access Point / Web Server the animations run a bit slower.

  #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))





  class ESPFastLedDriver
  {
  public:
    ESPFastLedDriver();
    void setup();
    uint8_t getPower();
    void setPower(uint8_t power);
    uint8_t getBrightness();
    void adjustBrightness(bool up);
    void setBrightness(uint8_t value);
    void adjustSpeed(bool up);
    uint8_t getSpeed(String speedType);
    void setSpeed(String type, uint8_t value);
    void adjustPattern(bool up);
    void setPattern(uint8_t value);
    uint8_t getCurrentPattern();
    String getCurrentPatternJson();
    void setPalette(uint8_t value);
    uint8_t getCurrentPalette();
    void setHue(uint8_t value);
    uint8_t getHue();
    void setSolidColor(CRGB color);
    void setSolidColor(uint8_t r, uint8_t g, uint8_t b);
    struct CRGB getSolidColor();
    String getSolidColorHex();
    String getStatus();
    CRGBPalette16 _gCurrentPalette;
    uint8_t getGHue();
    void loadSettings();
    struct CRGB getLeds();
    struct CRGB _leds[NUM_LEDS];
    struct CRGB hexToRGB(String hex);
    String RGBToHex(struct CRGB rgbColor);
    void setDelay(uint8_t delayType, uint8_t value);
    void updateTimers();

    void ack();

    void run(uint8_t delay);
  private:

    uint8_t _power;

    uint8_t _patternIndex;

    bool _powerIsDown = false;
    static const uint8_t _brightnessCount = 5;
    uint8_t _brightnessMap[_brightnessCount] = { 16, 32, 64, 128, 255 };
    int _brightnessIndex;
    uint8_t _brightness = _brightnessMap[_brightnessIndex];

    // static const uint8_t _speedCount = 5;
    // uint8_t _speedMap[_speedCount] = { 20, 30, 40, 50, 60 };
    // int _speedIndex = 3;
    // uint8_t _speed = _speedMap[_speedIndex];
    // uint8_t _speed = 0;

    int _gHueSpeed = 20;
    unsigned long _gHueSpeedPrev = 0;        // will store last time LED was updated

    int _pChangeSpeed = 3000;
    unsigned long _pChangeSpeedPrev = 0;        // will store last time LED was updated
    int _pBlendingSpeed = 10000;
    unsigned long _pBlendingSpeedPrev = 0;        // will store last time LED was updated
    // Current palette number from the 'playlist' of color palettes
    uint8_t _gCurrentPaletteNumber = 0;
    //
    CRGBPalette16 _gTargetPalette;

    uint8_t _currentPatternIndex = 0; // Index number of which pattern is current
    bool _autoplayEnabled = false;

    uint8_t _autoPlayDurationSeconds = 120;
    unsigned int _autoPlayTimeout = 0;

    uint8_t _gHue = 0; // rotating "base color" used by many of the patterns

    struct CRGB _solidColor;



    // uint8_t _patternCount = 4;


    // typedef void (*Pattern)();
    // // typedef void (ESPFastLedDriver::* Pattern)();
    // typedef Pattern PatternList[];
    //
    // typedef struct {
    //   Pattern pattern;
    //   String name;
    // } PatternAndName;
    // typedef PatternAndName PatternAndNameList[];

    char* _patterns[11] = {
    "Solid Color", "Color Waves", "Rainbow",  "Rainbow With Glitter", "Confetti", "Sinelon", "Juggle", "Pride", "Color BPM", "Palette Test", "Fire"
    };
    /* Animations */
    void showSolidColor();
    void rainbow();
    void rainbowWithGlitter();
    void confetti();
    void sinelon();
    void bpm();
    void juggle();
    void pride();
    void colorwaves();
    void palettetest();

    void fire(int Cooling, int Sparking, int SpeedDelay);
    // void fire(int Cooling, int Sparking);
    void setPixelHeatColor (int Pixel, byte temperature);
    void addGlitter(fract8 chanceOfGlitter);
    // PatternAndNameList _patterns;
    uint8_t _patternCount = 11;
  // uint8_t _patternCount = 4;

    // char _patterns[11] =
    // List of patterns to cycle through.  Each is defined as a separate function below.
    // PatternAndNameList _patterns = {
    //   { AnimColorWaves::run, "Color Waves" },
    // //   { palettetest, "Palette Test" },
    // //   { pride, "Pride" },
    // //   { rainbow, "Rainbow" },
    // //   { rainbowWithGlitter, "Rainbow With Glitter" },
    // //   { confetti, "Confetti" },
    // //   { sinelon, "Sinelon" },
    // //   { juggle, "Juggle" },
    // //   { bpm, "BPM" },
    // //   { showSolidColor, "Solid Color" },
    // };

    // const uint8_t patternCount = ARRAY_SIZE(patterns);
    // void setPatterns();

  };
  #endif
