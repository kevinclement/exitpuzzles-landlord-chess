#include "Arduino.h"
#include "cabinetLed.h"
#include "logic.h"

CabinetLed::CabinetLed(Logic &logic)
: _logic(logic)
{
}

void CabinetLed::setup() {
  pinMode(CABINET_LED_PIN, OUTPUT);
}

void CabinetLed::handle() {
  digitalWrite(CABINET_LED_PIN, enabled ? HIGH : LOW);
}