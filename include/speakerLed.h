#pragma once

#include "Arduino.h"

class Logic;

#define SPEAKER_LED_PIN  33

class SpeakerLed {
  public:
    bool enabled = false;
    
    SpeakerLed(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};