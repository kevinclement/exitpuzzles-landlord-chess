#pragma once

#include "Arduino.h"
#include "pn532Device.h"

class Logic;

class Rfid {
  public:
    PN532Device nfc1;
    PN532Device nfc2;

    Rfid(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};