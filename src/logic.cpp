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
  
  // check for bust
  // TODO: KYLE: do I want to allow it to be turned on and off?
  // if (bust.isSwitched && !lights.) {

  // }

  // check for second final solved state
  if (!solved && rfid.nfc1.state == CORRECT && rfid.nfc2.state == CORRECT) {
    Serial.println("Both RFIDs correct.  Puzzle solved.");
    triggerSecond();
    solved = true;
    status();
  }

  // Magnet will be on when switch is disabled
  // magnet.enabled = !bust.isSwitched;


  // TODO: do I have to worry about making sure there isn't a conflict with RFID and bust both going off at the same time?
  // the odds of that are probably pretty low.
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "piece_1:%s,"
      "piece_2:%s,"
      "bust:%s,"
      "magnet:%s,"
      "solved:%s"

      "\r\n"
    ,
      rfid.nfc1.state == CORRECT ? "true" : "false",
      rfid.nfc2.state == CORRECT ? "true" : "false", 
      bust.isSwitched ? "on" : "off",
      magnet.enabled ? "enabled" : "disabled",
      solved ? "true" : "false"
  );

  Serial.print(cMsg);
}

void Logic::triggerFirst() {
  sound.bustTriggered();
  lights.triggerFirst();
}

void Logic::triggerSecond() {
  sound.solved();
  lights.triggerSecond();
}
