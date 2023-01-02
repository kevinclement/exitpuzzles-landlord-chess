#include "Arduino.h"
#include "rfid.h"
#include "logic.h"

Rfid::Rfid(Logic &logic)
: _logic(logic),

  // F2 (BLACK-QUEEN)
  nfc1(logic, A3, 21, "F2", F2),

  // C7 (WHITE-KING)
  nfc2(logic, A4, A5, "C7", C7)
{
}

void Rfid::setup() {
  nfc1.setup();
  nfc2.setup();

  Serial.println();
}

void Rfid::handle() {
  nfc1.handle();
  nfc2.handle();
}