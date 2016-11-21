#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>
#include <ESPLedDriver.h>
// #include <AnimationColorWaves.h>

//abstract class Sensor
class Animation {
    public:
        //initialize the sensor
        inline virtual void begin(){ /*nothing*/ };
        //read function must be implemented
        //this is called a pure virtual function
        virtual int run(ESPLedDriver* &ledDriver) = 0;
};
class AnimationColorWaves : public Animation {
    public:
        inline AnimationColorWaves( ) {}
        //read function must be implemented
        virtual int run(ESPLedDriver* &ledDriver);
};

#endif
