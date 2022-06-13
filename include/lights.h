#pragma once

#include "Arduino.h"

#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS            32
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
#define DATA_PIN            A1

class Logic;

enum LIGHTS_STATE {INIT, RUNNING, SOLVED};

class Lights {
  public:
    bool enabled = false;
    LIGHTS_STATE first = INIT;
    LIGHTS_STATE second = INIT;

    Lights(Logic &logic);
    void setup();
    void handle();
    void triggerFirst();
    void triggerSecond();
    void triggerAttractMode();

  private:
    Logic &_logic;

    bool attractMode    = true;
    bool solvingFirst   = false;
    bool solvedFirst    = false;
    bool solvingSecond  = false;
    bool solvedSecond   = false;

    void fadeInAndOut();
    void sweep();
    void fancyPants();
};