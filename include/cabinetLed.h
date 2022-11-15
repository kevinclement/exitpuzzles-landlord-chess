#pragma once

#include "Arduino.h"

class Logic;

#define CABINET_LED_PIN  13

class CabinetLed {
  public:
    bool enabled = true;
    
    CabinetLed(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    bool _prevEnabled = true;
};