#pragma once

#include "Arduino.h"

#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS            32
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
#define DATA_PIN            A1

class Logic;

class Lights {
  public:
    bool enabled = false;
    bool firstHalfSolved = false;
    bool secondHalfSolved = true;
    
    Lights(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    void fadeInAndOut();
};