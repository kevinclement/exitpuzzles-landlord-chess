#include "Arduino.h"
#include "rfid.h"
#include "logic.h"

#define NUM_VALID_TAGS 2
byte VALID_TAGS [2][NUM_VALID_TAGS][4] = {

  // F2|WHITE tags
  {
    { 0x04, 0x16, 0x22, 0x0A }, // KING
    { 0x04, 0xBF, 0x21, 0x0A }  // QUEEN
  },

  // C7|BLACK tags
  {
    { 0x04, 0xC7, 0x21, 0x0A },  // KING 
    { 0x04, 0x97, 0x1F, 0x0A }   // QUEEN
  }
};

Rfid::Rfid(Logic &logic)
: _logic(logic),

  // F2
  nfc1(logic, A3, 21, "NFC1", VALID_TAGS[0], NUM_VALID_TAGS),

  // C7
  nfc2(logic, A4, A5, "NFC2", VALID_TAGS[1], NUM_VALID_TAGS)
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