#pragma once

#include "Arduino.h"

class Logic;

#define CABINET_PIN  15

class Cabinet {
  public:
    bool enabled = true;
    
    Cabinet(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    bool _prevEnabled = true;
};