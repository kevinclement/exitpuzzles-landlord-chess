#pragma once

#include "Arduino.h"

class Logic;

#define CABINET_LED_PIN  13

class CabinetLed {
  public:
    bool enabled = false;
    bool blinking = false;
    
    CabinetLed(Logic &logic);
    void setup();
    void handle();
    void blinkOn();
    void blinkOff();

  private:
    Logic &_logic;
};