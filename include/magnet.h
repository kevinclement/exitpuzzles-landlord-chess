#pragma once

#include "Arduino.h"

class Logic;

#define MAGNET_PIN  32
#define ON          LOW
#define OFF         HIGH

class Magnet {
  public:
    bool enabled = false;
    
    Magnet(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    bool _prevEnabled = false;
};