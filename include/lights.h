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
    
    Lights(Logic &logic);
    void setup();
    void handle();
    void solvedFirst();
    void solvedSecond();

  private:
    Logic &_logic;


    bool solvingFirst = false;
    bool firstHalfSolved = false;

    bool solvingSecond = false;
    bool secondHalfSolved = false;

    void fadeInAndOut();
    void sweep();
};