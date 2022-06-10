#include "Arduino.h"
#include "logic.h"
#include "version.h"

Logic::Logic()
  : serial(),
    rfid(*this), 
    bust(*this),
    magnet(*this),
    lights(*this),
    sound(*this),
    pn532(*this)
{
}

void Logic::setup() {
  serial.setup("");
  rfid.setup();
  bust.setup();
  magnet.setup();
  lights.setup();
  sound.setup();
  pn532.setup();
}

void Logic::handle() {
  rfid.handle();
  bust.handle();
  magnet.handle();
  sound.handle();
  lights.handle();
  pn532.handle();

  // Magnet will be on when switch is disabled
  magnet.enabled = !bust.isSwitched;
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
      "bust:%s,"
      "magnet:%s"

      "\r\n"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      rfid.state[0] == CORRECT ? "true" : "false",
      rfid.state[1] == CORRECT ? "true" : "false", 
      bust.isSwitched ? "on" : "off",
      magnet.enabled ? "enabled" : "disabled"
  );

  Serial.print(cMsg);
}
