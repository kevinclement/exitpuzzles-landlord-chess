#pragma once

#include "Arduino.h"
#include "rfid.h"
#include "bust.h"
#include "magnet.h"
#include "SerialManager.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  Rfid rfid;
  Bust bust;
  Magnet magnet;
  bool debug;

  void setup();
  void handle();
  void status();

private:
};

