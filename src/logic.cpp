#include "Arduino.h"
#include "logic.h"
#include "rfid.h"
#include "version.h"

Logic::Logic()
  : serial(),
    rfid(*this), 
    bust(*this)
{
}

void Logic::setup() {
  serial.setup("");
  rfid.setup();
  bust.setup();
}

void Logic::handle() {
  rfid.handle();
  bust.handle();
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "piece_1:%s,"
      "piece_2:%s,"
      "bust:%s"

      "\r\n"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      rfid.state[0] == CORRECT ? "true" : "false",
      rfid.state[1] == CORRECT ? "true" : "false", 
      bust.isSwitched ? "true" : "false"
  );

  Serial.print(cMsg);
}
