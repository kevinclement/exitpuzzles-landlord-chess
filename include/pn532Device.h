#pragma once

#include "Arduino.h"

class Logic;

class PN532Device {
  public:
    PN532Device(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};