#pragma once

#include "Arduino.h"

class Logic;

#define SPEAKER_LED_PIN  33

class SpeakerLed {
  public:
    SpeakerLed(Logic &logic);
    bool enabled = false;
    
    void setup();
    void handle();
    void blinkOn();
    void blinkOff();

  private:
    Logic &_logic;
    bool blinking = false;
};