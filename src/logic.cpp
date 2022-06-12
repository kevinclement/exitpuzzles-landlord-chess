#include "Arduino.h"
#include "logic.h"

Logic::Logic()
  : rfid(*this), 
    bust(*this),
    magnet(*this),
    lights(*this),
    sound(*this)
{
}

void Logic::setup() {
  rfid.setup();
  bust.setup();
  magnet.setup();
  lights.setup();
  sound.setup();
}

void Logic::handle() {
  rfid.handle();
  bust.handle();
  magnet.handle();
  sound.handle();
  lights.handle();
  
  // Magnet will be on when switch is disabled
  magnet.enabled = !bust.isSwitched;
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "piece_1:%s,"
      "piece_2:%s,"
      "bust:%s,"
      "magnet:%s"

      "\r\n"
    ,
      rfid.nfc1.state == CORRECT ? "true" : "false",
      rfid.nfc2.state == CORRECT ? "true" : "false", 
      bust.isSwitched ? "on" : "off",
      magnet.enabled ? "enabled" : "disabled"
  );

  Serial.print(cMsg);
}

void Logic::triggerFirst() {
  lights.triggerFirst();
}

void Logic::triggerSecond() {
  lights.triggerSecond();
}
