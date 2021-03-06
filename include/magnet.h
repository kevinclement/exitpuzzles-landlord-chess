#pragma once

#include "Arduino.h"

class Logic;

#define MAGNET_PIN  32

class Magnet {
  public:
    bool enabled = true;
    
    Magnet(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    bool _prevEnabled = true;
};