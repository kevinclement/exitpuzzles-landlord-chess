#include "Arduino.h"
#include "rfid.h"
#include "logic.h"

#define NUM_VALID_TAGS 2
byte VALID_TAGS [2][NUM_VALID_TAGS][4] = {
  {
    { 0xF7, 0x6F, 0x8C, 0xF2 },
    { 0x87, 0x93, 0x8E, 0xF2 }
  },
  {
    { 0x27, 0x8F, 0x8E, 0xF2 },
    { 0xE3, 0xB8, 0xE0, 0x91 }
  }
};

Rfid::Rfid(Logic &logic)
: _logic(logic),
  nfc1(logic, A3, 21, "NFC1", VALID_TAGS[0], NUM_VALID_TAGS),
  nfc2(logic, A4, A5, "NFC2", VALID_TAGS[1], NUM_VALID_TAGS)
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