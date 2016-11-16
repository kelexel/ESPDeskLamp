#include <Arduino.h>
#include "ESPIRDriver.h"

ESPIRDriver::ESPIRDriver(IRrecv* irReceiver, ESPLedDriver* ledDriver) {
  _ledDriver = ledDriver;
  _irReceiver = irReceiver;
}

void ESPIRDriver::run()
{
  // InputCommand command = readCommand(defaultHoldDelay);
  //
  // if (command != InputCommand::None) {
  //   Serial.print("command: ");
  //   Serial.println((int) command);
  // }
  //
  // switch (command) {
  //   case InputCommand::Up: {
  //       _ledDriver->adjustPattern(true);
  //       break;
  //     }
  //   case InputCommand::Down: {
  //       _ledDriver->adjustPattern(false);
  //       break;
  //     }
  //   case InputCommand::Power: {
  //       _ledDriver->setPower(_ledDriver->getPower() == 0 ? 1 : 0);
  //       break;
  //     }
  //   case InputCommand::BrightnessUp: {
  //       _ledDriver->adjustBrightness(true);
  //       break;
  //     }
  //   case InputCommand::BrightnessDown: {
  //       _ledDriver->adjustBrightness(false);
  //       break;
  //     }
  //   case InputCommand::PlayMode: { // toggle pause/play
  //       // autoplayEnabled = !autoplayEnabled;
  //       break;
  //     }
  //
  //   // pattern buttons
  //
  //   case InputCommand::Pattern1: {
  //       _ledDriver->setPattern(0);
  //       break;
  //     }
  //   case InputCommand::Pattern2: {
  //       _ledDriver->setPattern(1);
  //       break;
  //     }
  //   case InputCommand::Pattern3: {
  //       _ledDriver->setPattern(2);
  //       break;
  //     }
  //   case InputCommand::Pattern4: {
  //       _ledDriver->setPattern(3);
  //       break;
  //     }
  //   case InputCommand::Pattern5: {
  //       _ledDriver->setPattern(4);
  //       break;
  //     }
  //   case InputCommand::Pattern6: {
  //       _ledDriver->setPattern(5);
  //       break;
  //     }
  //   case InputCommand::Pattern7: {
  //       _ledDriver->setPattern(6);
  //       break;
  //     }
  //   case InputCommand::Pattern8: {
  //       _ledDriver->setPattern(7);
  //       break;
  //     }
  //   case InputCommand::Pattern9: {
  //       _ledDriver->setPattern(8);
  //       break;
  //     }
  //   case InputCommand::Pattern10: {
  //       _ledDriver->setPattern(9);
  //       break;
  //     }
  //   case InputCommand::Pattern11: {
  //       _ledDriver->setPattern(10);
  //       break;
  //     }
  //   case InputCommand::Pattern12: {
  //       _ledDriver->setPattern(11);
  //       break;
  //     }
  //
  //   // custom color adjustment buttons
  //
  //   case InputCommand::RedUp: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.red += 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //   case InputCommand::RedDown: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.red -= 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //   case InputCommand::GreenUp: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.green += 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //   case InputCommand::GreenDown: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.green -= 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //   case InputCommand::BlueUp: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.blue += 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //   case InputCommand::BlueDown: {
  //       CRGB solidColor = _ledDriver->getSolidColor();
  //       solidColor.blue -= 8;
  //       _ledDriver->setSolidColor(solidColor);
  //       break;
  //     }
  //
  //   // color buttons
  //
  //   case InputCommand::Red: {
  //       _ledDriver->setSolidColor(CRGB::Red);
  //       break;
  //     }
  //   case InputCommand::RedOrange: {
  //       _ledDriver->setSolidColor(CRGB::OrangeRed);
  //       break;
  //     }
  //   case InputCommand::Orange: {
  //       _ledDriver->setSolidColor(CRGB::Orange);
  //       break;
  //     }
  //   case InputCommand::YellowOrange: {
  //       _ledDriver->setSolidColor(CRGB::Goldenrod);
  //       break;
  //     }
  //   case InputCommand::Yellow: {
  //       _ledDriver->setSolidColor(CRGB::Yellow);
  //       break;
  //     }
  //
  //   case InputCommand::Green: {
  //       _ledDriver->setSolidColor(CRGB::Green);
  //       break;
  //     }
  //   case InputCommand::Lime: {
  //       _ledDriver->setSolidColor(CRGB::Lime);
  //       break;
  //     }
  //   case InputCommand::Aqua: {
  //       _ledDriver->setSolidColor(CRGB::Aqua);
  //       break;
  //     }
  //   case InputCommand::Teal: {
  //       _ledDriver->setSolidColor(CRGB::Teal);
  //       break;
  //     }
  //   case InputCommand::Navy: {
  //       _ledDriver->setSolidColor(CRGB::Navy);
  //       break;
  //     }
  //
  //   case InputCommand::Blue: {
  //       _ledDriver->setSolidColor(CRGB::Blue);
  //       break;
  //     }
  //   case InputCommand::RoyalBlue: {
  //       _ledDriver->setSolidColor(CRGB::RoyalBlue);
  //       break;
  //     }
  //   case InputCommand::Purple: {
  //       _ledDriver->setSolidColor(CRGB::Purple);
  //       break;
  //     }
  //   case InputCommand::Indigo: {
  //       _ledDriver->setSolidColor(CRGB::Indigo);
  //       break;
  //     }
  //   case InputCommand::Magenta: {
  //       _ledDriver->setSolidColor(CRGB::Magenta);
  //       break;
  //     }
  //
  //   case InputCommand::White: {
  //       _ledDriver->setSolidColor(CRGB::White);
  //       break;
  //     }
  //   case InputCommand::Pink: {
  //       _ledDriver->setSolidColor(CRGB::Pink);
  //       break;
  //     }
  //   case InputCommand::LightPink: {
  //       _ledDriver->setSolidColor(CRGB::LightPink);
  //       break;
  //     }
  //   case InputCommand::BabyBlue: {
  //       _ledDriver->setSolidColor(CRGB::CornflowerBlue);
  //       break;
  //     }
  //   case InputCommand::LightBlue: {
  //       _ledDriver->setSolidColor(CRGB::LightBlue);
  //       break;
  //     }
  // }
}
