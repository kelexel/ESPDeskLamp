#include <Arduino.h>
#include "ESPLedDriver.h"
#include "GradientPalettes.h"


ESPLedDriver::ESPLedDriver() {
    setPower(1);
    setBrightness(100);
    _gCurrentPalette = CRGBPalette16( CRGB::Black);
    _gTargetPalette = CRGBPalette16( gGradientPalettes[0] );
}

void ESPLedDriver::setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS);         // for WS2812 (Neopixel)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(_brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(_leds, NUM_LEDS, _solidColor);
  FastLED.show();
}

void ESPLedDriver::loadSettings()
{
  _brightness = EEPROM.read(0);

  _currentPatternIndex = EEPROM.read(1);
  if (_currentPatternIndex < 0)
    _currentPatternIndex = 0;
  else if (_currentPatternIndex >= _patternCount)
    _currentPatternIndex = _patternCount - 1;

  byte r = EEPROM.read(2);
  byte g = EEPROM.read(3);
  byte b = EEPROM.read(4);

  _speed = EEPROM.read(5);

  if (r == 0 && g == 0 && b == 0)
  {
  }
  else
  {
    _solidColor = CRGB(r, g, b);
  }
}



/*
Power
*/

uint8_t ESPLedDriver::getPower() {
  return _power;
}
void ESPLedDriver::setPower(uint8_t value) {
  _power = value;
}



/*
brightness
*/

uint8_t ESPLedDriver::getBrightness() {
  return _brightness;
}
// adjust the brightness, and wrap around at the ends
void ESPLedDriver::adjustBrightness(bool up)
{
  if (up)
    _brightnessIndex++;
  else
    _brightnessIndex--;

  // wrap around at the ends
  if (_brightnessIndex < 0)
    _brightnessIndex = _brightnessCount - 1;
  else if (_brightnessIndex >= _brightnessCount)
    _brightnessIndex = 0;

  _brightness = _brightnessMap[_brightnessIndex];

  FastLED.setBrightness(_brightness);

  EEPROM.write(0, _brightness);
  EEPROM.commit();
}

void ESPLedDriver::setBrightness(uint8_t value) {
  // don't wrap around at the ends
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  _brightness = value;

  FastLED.setBrightness(_brightness);

  EEPROM.write(0, _brightness);
  EEPROM.commit();
}




/*
Speed
*/

// adjust the speed, and wrap around at the ends
void ESPLedDriver::adjustSpeed(bool up)
{
  if (up)
    _speedIndex++;
  else
    _speedIndex--;

  // wrap around at the ends
  if (_speedIndex < 0)
    _speedIndex = _speedCount - 1;
  else if (_speedIndex >= _speedCount)
    _speedIndex = 0;

  _speed = _speedMap[_speedIndex];

//  FastLED.setBrightness(brightness);

//  EEPROM.write(0, brightness);
//  EEPROM.commit();
}

void ESPLedDriver::setSpeed(uint8_t value)
{
  // don't wrap around at the ends
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  _speed = value;

//  FastLED.setSpeed(speed);

//  EEPROM.write(0, brightness);
//  EEPROM.commit();
}



void ESPLedDriver::setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void ESPLedDriver::setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  _solidColor = CRGB(r, g, b);

  EEPROM.write(2, r);
  EEPROM.write(3, g);
  EEPROM.write(4, b);

  setPattern(_patternCount - 1);
}
CRGB ESPLedDriver::getSolidColor()
{
  return _solidColor;
}
// increase or decrease the current pattern number, and wrap around at the ends
void ESPLedDriver::adjustPattern(bool up)
{
  if (up)
    _currentPatternIndex++;
  else
    _currentPatternIndex--;

  // wrap around at the ends
  if (_currentPatternIndex < 0)
    _currentPatternIndex = _patternCount - 1;
  if (_currentPatternIndex >= _patternCount)
    _currentPatternIndex = 0;

  EEPROM.write(1, _currentPatternIndex);
  EEPROM.commit();
}

void ESPLedDriver::setPattern(int value)
{
  // don't wrap around at the ends
  if (value < 0)
    value = 0;
  else if (value >= _patternCount)
    value = _patternCount - 1;

  _currentPatternIndex = value;

  EEPROM.write(1, _currentPatternIndex);
  EEPROM.commit();
}





/*
animations
*/

void ESPLedDriver::showSolidColor()
{
  fill_solid(_leds, NUM_LEDS, _solidColor);
}

void ESPLedDriver::rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( _leds, NUM_LEDS, _gHue, 10);
}

void ESPLedDriver::rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void ESPLedDriver::addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    _leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void ESPLedDriver::confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  _leds[pos] += CHSV( _gHue + random8(64), 200, 255);
}

void ESPLedDriver::sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( _leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  _leds[pos] += CHSV( _gHue, 255, 192);
}

void ESPLedDriver::bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    _leds[i] = ColorFromPalette(palette, _gHue + (i * 2), beat - _gHue + (i * 10));
  }
}

void ESPLedDriver::juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( _leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++)
  {
    _leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// Pride2015 by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void ESPLedDriver::pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    nblend(_leds[i], newcolor, 64);
  }
}





// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void ESPLedDriver::colorwaves()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  // uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette(_gCurrentPalette, index, bri8);

    nblend(_leds[i], newcolor, 128);
  }
}

// Alternate rendering function just scrolls the current palette
// across the defined LED strip.
void ESPLedDriver::palettetest()
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( _leds, NUM_LEDS, startindex, (256 / NUM_LEDS) + 1, _gCurrentPalette, 255, LINEARBLEND);
}



String ESPLedDriver::getStatus()
{
  String json = "{";

  json += "\"power\":" + String(_power) + ",";
  json += "\"brightness\":" + String(_brightness) + ",";
  json += "\"speed\":" + String(_speed) + ",";

  json += "\"currentPattern\":{";
  json += "\"index\":" + String(_currentPatternIndex);
  //json += ",\"name\":\"" + _patterns[_currentPatternIndex].name + "\"}";

  json += ",\"solidColor\":{";
  json += "\"r\":" + String(_solidColor.r);
  json += ",\"g\":" + String(_solidColor.g);
  json += ",\"b\":" + String(_solidColor.b);
  json += "}";

  json += ",\"patterns\":[";
  // for (uint8_t i = 0; i < _patternCount; i++)
  // {
  //   json += "\"" + _patterns[i].name + "\"";
  //   if (i < _patternCount - 1)
  //     json += ",";
  // }
  json += "]";

  json += "}";
  return json;
}










void ESPLedDriver::run() {
  if (_power == 0) {
    fill_solid(_leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(15);
    return;
  }
}
