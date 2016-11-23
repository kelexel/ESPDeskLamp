#include <Arduino.h>
#include <math.h>
#include "ESPFastLedDriver.h"
#include <GradientPalettes.h>


ESPFastLedDriver::ESPFastLedDriver() {
    _gCurrentPalette = CRGBPalette16( CRGB::Black);
    _gTargetPalette = CRGBPalette16( gGradientPalettes[0] );
}

// void ESPFastLedDriver::setup() {
//   FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS);         // for WS2812 (Neopixel)
//   //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
//   FastLED.setCorrection(TypicalLEDStrip);
//   FastLED.setBrightness(_brightness);
//   FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
//   Serial.print('TEST ');
//   Serial.println(String(_solidColor));
//   _solidColor = CRGB::Blue;
//   struct CRGB solidColor = CRGB::Blue;
//   Serial.print('TEST2 ');
//   Serial.println(String(solidColor));
//
//   fill_solid(_leds, NUM_LEDS, _solidColor);
//   FastLED.show();
// }
//
void ESPFastLedDriver::setup() {
  // setPower(1);
  // setBrightness(100);
  loadSettings();
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS);         // for WS2812 (Neopixel)

  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);

  FastLED.setBrightness(_brightness);
  fill_solid(_leds, NUM_LEDS, _solidColor);

  FastLED.show();
}

// void ESPFastLedDriver::loadSettings()
// {
//   _brightness = EEPROM.read(0);
//
//   _currentPatternIndex = EEPROM.read(1);
//   if (_currentPatternIndex < 0)
//     _currentPatternIndex = 0;
//   else if (_currentPatternIndex >= _patternCount)
//     _currentPatternIndex = _patternCount - 1;
//
//   byte r = EEPROM.read(2);
//   byte g = EEPROM.read(3);
//   byte b = EEPROM.read(4);
//
//   _speed = EEPROM.read(5);
//
//   if (r == 0 && g == 0 && b == 0)
//   {
//   }
//   else
//   {
//     _solidColor = CRGB(r, g, b);
//   }
// }

void ESPFastLedDriver::loadSettings()
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

  if (_speed = 0) {
    _speed = 40;
    _speedIndex = 3;
  }

  if (r == 0 && g == 0 && b == 0)
  {
    _solidColor = CRGB::Blue;
  }
  else
  {
    _solidColor = CRGB(r, g, b);
  }
}



/*
Power
*/

uint8_t ESPFastLedDriver::getPower() {
  return _power;
}
void ESPFastLedDriver::setPower(uint8_t value) {
  if (_power != value) {
    _power = value;
    _powerIsDown = false;
    if (value == 1) {
      fill_solid(_leds, NUM_LEDS, _solidColor);
    }
  }
}



/*
brightness
*/

uint8_t ESPFastLedDriver::getBrightness() {
  return round(_brightness / 2.5);
}
// adjust the brightness, and wrap around at the ends
void ESPFastLedDriver::adjustBrightness(bool up)
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

void ESPFastLedDriver::setBrightness(uint8_t value) {
  // don't wrap around at the ends
  value = round(value * 2.5);
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  _brightness = value;
  Serial.print("B: ");
  Serial.println(String(value));

  FastLED.setBrightness(_brightness);

  EEPROM.write(0, _brightness);
  EEPROM.commit();
}




/*
Speed
*/

// adjust the speed, and wrap around at the ends
void ESPFastLedDriver::adjustSpeed(bool up)
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

void ESPFastLedDriver::setSpeed(uint8_t value)
{
  // don't wrap around at the ends
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  _speed = value;

 EEPROM.write(5, _speed);
 EEPROM.commit();
}



void ESPFastLedDriver::setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void ESPFastLedDriver::setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  _solidColor = CRGB(r, g, b);

  EEPROM.write(2, r);
  EEPROM.write(3, g);
  EEPROM.write(4, b);

  fill_solid(_leds, NUM_LEDS, _solidColor);
  setPattern(0);
}
CRGB ESPFastLedDriver::getSolidColor()
{
  return _solidColor;
}
String ESPFastLedDriver::getSolidColorHex()
{
  char hex[7] = {0};
  sprintf(hex,"%02X%02X%02X",_solidColor.r,_solidColor.g,_solidColor.b); //convert to an hexadecimal string. Lookup sprintf for what %02X means.
  return hex;
}
// increase or decrease the current pattern number, and wrap around at the ends
void ESPFastLedDriver::adjustPattern(bool up)
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
uint8_t ESPFastLedDriver::getCurrentPattern()
{
  return _currentPatternIndex;
}
String ESPFastLedDriver::getCurrentPatternJson()
{
  String json = "{";
  json += "\"index\":" + String(_currentPatternIndex);
  json += ",\"name\":\"" + String(_patterns[_currentPatternIndex]) + "\"";
  json += "}";
  return json;
}


void ESPFastLedDriver::setPattern(int value)
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
// void ESPFastLedDriver::setPatterns()
// {
//   // AnimColorWaves* colorWaves = new AnimColorWaves();
  // PatternAndNameList _patterns = {
  //   // { AnimColorWaves::run, "Color Waves" },
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
//
//   // { int new AnimColorWaves(), "Color Waves" },
//
//   // const uint8_t patternCount;
//   // const PROGMEM  String  variableName[] = {
//   //   "a", "b"
//   // }; // or this form
//   //
//   // int patterns[2] = {
//   //
//   //   // no luck: src/ESPFastLedDriver.cpp:225:3: error: cannot convert 'ESPFastLedDriver::colorwaves' from type 'void (ESPFastLedDriver::)()' to type 'ESPFastLedDriver::Pattern {aka void (*)()}'
//   //   { colorwaves, "Color Waves" },
//   //
//   //   // no luck: src/ESPFastLedDriver.cpp:225:3: error: cannot convert 'ESPFastLedDriver::palettetest' from type 'void (ESPFastLedDriver::)()' to type 'ESPFastLedDriver::Pattern {aka void (*)()}'
//   //   { this->palettetest, "Palette Test" }
//   //
//   //   // { this->pride, "Pride" },
//   //   // { this->rainbow, "Rainbow" },
//   //   // { this->rainbowWithGlitter, "Rainbow With Glitter" },
//   //   // { this->confetti, "Confetti" },
//   //   // { this->sinelon, "Sinelon" },
//   //   // { this->juggle, "Juggle" },
//   //   // { this->bpm, "BPM" },
//   //   // { this->showSolidColor, "Solid Color" },
//   // };
// }
void ESPFastLedDriver::ack()
{
  addGlitter(100);
}
void ESPFastLedDriver::showSolidColor()
{
  fill_solid(_leds, NUM_LEDS, _solidColor);
}

void ESPFastLedDriver::rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( _leds, NUM_LEDS, _gHue, 10);
}

void ESPFastLedDriver::rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void ESPFastLedDriver::addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    _leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void ESPFastLedDriver::confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  _leds[pos] += CHSV( _gHue + random8(64), 200, 255);
}

void ESPFastLedDriver::sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( _leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  _leds[pos] += CHSV( _gHue, 255, 192);
}

void ESPFastLedDriver::bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    _leds[i] = ColorFromPalette(palette, _gHue + (i * 2), beat - _gHue + (i * 10));
  }
}

void ESPFastLedDriver::juggle()
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
void ESPFastLedDriver::pride() {
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
void ESPFastLedDriver::colorwaves()
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
void ESPFastLedDriver::palettetest()
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( _leds, NUM_LEDS, startindex, (256 / NUM_LEDS) + 1, _gCurrentPalette, 255, LINEARBLEND);
}

// adapted from http://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#fire
void ESPFastLedDriver::fire(int Cooling, int Sparking, int SpeedDelay) {
// void ESPFastLedDriver::fire(int Cooling, int Sparking) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  FastLED.delay(SpeedDelay);
}

void ESPFastLedDriver::setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    // setPixel(Pixel, 255, 255, heatramp);
    _leds[Pixel] = CRGB( 255, 255, heatramp);

  } else if( t192 > 0x40 ) {             // middle
    // setPixel(Pixel, 255, heatramp, 0);
    _leds[Pixel] = CRGB( 255, heatramp, 0);
  } else {                               // coolest
    // setPixel(Pixel, heatramp, 0, 0);
    _leds[Pixel] = CRGB( heatramp, 0, 0);
  }
}








// struct CRGBPalette16 ESPFastLedDriver::getCurrentPalette() {
//   return _gCurrentPalette;
// }
uint8_t ESPFastLedDriver::getGHue() {
  return _gHue;
}
uint8_t ESPFastLedDriver::getSpeed() {
  return _speed;
}
struct CRGB ESPFastLedDriver::getLeds() {
  // return _leds;
}

String ESPFastLedDriver::getStatus()
{
  String json = "{";

  json += "\"power\":" + String(_power) + ",";
  json += "\"brightness\":" + String(_brightness) + ",";
  json += "\"speed\":" + String(_speed) + ",";

  json += "\"currentPattern\":{";
  json += "\"index\":" + String(_currentPatternIndex);
  json += ",\"name\":\"" + String(_patterns[_currentPatternIndex]) + "\"}";

  json += ",\"solidColor\":{";
  json += "\"r\":" + String(_solidColor.r);
  json += ",\"g\":" + String(_solidColor.g);
  json += ",\"b\":" + String(_solidColor.b);
  json += "}";

  json += ",\"patterns\":[";
  for (uint8_t i = 0; i < _patternCount; i++)
  {
    json += "\"" + String(_patterns[i]) + "\"";
    // json += "\"foo\"";
    if (i < _patternCount - 1)
      json += ",";
  }
  json += "]";

  json += "}";
  return json;
}






struct CRGB ESPFastLedDriver::hexToRGB(String hex) {
  int number = (int) strtol( &hex[0], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  struct CRGB rgbColor = CRGB(r, g, b);
  return rgbColor;
}
String ESPFastLedDriver::RGBToHex(struct CRGB rgbColor) {
  char hex[7] = {0};
  sprintf(hex,"%02X-%02X-%02X",rgbColor.r,rgbColor.g,rgbColor.b); //convert to an hexadecimal string. Lookup sprintf for what %02X means.
  return String(hex);
}


void ESPFastLedDriver::setPalette(uint8_t value) {

}
uint8_t ESPFastLedDriver::getCurrentPalette() {
  return _gCurrentPaletteNumber;
}
void ESPFastLedDriver::setHue(uint8_t value) {

}
uint8_t ESPFastLedDriver::getHue() {
  return _gHue;
}

void ESPFastLedDriver::run(uint8_t delay) {
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(65535));

  // EVERY_N_MILLISECONDS( 40 ) {
  EVERY_N_MILLISECONDS( _speed ) {
    _gHue++;  // slowly cycle the "base color" through the rainbow
  }

  // change to a new cpt-city gradient palette
  EVERY_N_SECONDS( _palettesPerSecond ) {
    _gCurrentPaletteNumber = addmod8( _gCurrentPaletteNumber, 1, gGradientPaletteCount);
    _gTargetPalette = gGradientPalettes[ _gCurrentPaletteNumber ];
  }

  // slowly blend the current cpt-city gradient palette to the next
  // EVERY_N_MILLISECONDS(80) {
  EVERY_N_MILLISECONDS(_speed*2) {
    nblendPaletteTowardPalette( _gCurrentPalette, _gTargetPalette, 16);
  }

  if (_autoplayEnabled && millis() > _autoPlayTimeout) {
    adjustPattern(true);
    _autoPlayTimeout = millis() + (_autoPlayDurationSeconds * 1000);
  }

  if (_power == 0 && !_powerIsDown) {
    // Serial.println("POWER OFF");
    fill_solid(_leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    _powerIsDown = true;
  } else if (_power == 0 && _powerIsDown) {
    // do nothing power is down!
  } else {
    switch(_currentPatternIndex) {
      case 0: showSolidColor(); break;
      case 1: colorwaves(); break;
      case 2: rainbow(); break;
      case 3: rainbowWithGlitter(); break;
      case 4: confetti(); break;
      case 5: sinelon(); break;
      case 6: bpm(); break;
      case 7: juggle(); break;
      case 8: pride(); break;
      case 9: palettetest(); break;
      case 10: fire(55,120,15); break;
    }
    FastLED.show();
    FastLED.delay(delay);
  }
}
