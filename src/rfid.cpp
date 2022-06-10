#include "Arduino.h"
#include "rfid.h"
#include "logic.h"

Rfid::Rfid(Logic &logic)
: _logic(logic),
  nfc1(logic, A3, 21, "NFC1"),
  nfc2(logic, A4, A5, "NFC2")
{
}

void Rfid::setup() {
  nfc1.setup();
  nfc2.setup();
}

void Rfid::handle() {
  nfc1.handle();
  nfc2.handle();
}