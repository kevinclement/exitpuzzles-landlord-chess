#pragma once

#include "Arduino.h"

class Logic;

#define SPEAKER_LED_PIN  33

class SpeakerLed {
  public:
    bool blinking = false;
    bool enabled = false;
    
    SpeakerLed(Logic &logic);
    void setup();
    void handle();
    void blinkOn();
    void blinkOff();

  private:
    Logic &_logic;
};