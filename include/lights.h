#pragma once

#include "Arduino.h"

#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS            32
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
#define DATA_PIN            A1

class Logic;

enum LIGHTS_STATE {OFF, ATTRACT, SWEEPING, PULSING};

class Lights {
  public:
    bool enabled = false;
    LIGHTS_STATE first = OFF;
    LIGHTS_STATE second = OFF;

    Lights(Logic &logic);
    void setup();
    void handle();
    void triggerFirst();
    void triggerSecond();
    void triggerAttractMode();
    void triggerLightsOff();

  private:
    Logic &_logic;

    unsigned long pulsing_time = 0;

    bool attractMode    = false;
    bool solvingFirst   = false;
    bool solvedFirst    = false;
    bool solvingSecond  = false;
    bool solvedSecond   = false;

    void fadeInAndOut();
    void sweep();
    void fancyPants();
};