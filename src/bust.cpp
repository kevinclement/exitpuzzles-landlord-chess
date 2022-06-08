#include "Arduino.h"
#include "bust.h"
#include "logic.h"

Bust::Bust(Logic &logic)
: _logic(logic)
{
}

void Bust::setup() {
  pinMode(BTN_SWITCH_PIN, INPUT_PULLUP);
}

void Bust::handle() {
  bool _prevSwitched = isSwitched;
  isSwitched = digitalRead(BTN_SWITCH_PIN);
  
  if (_prevSwitched != isSwitched) {
    Serial.print("switch: ");
    Serial.println(isSwitched ? "ON" : "OFF");
    _logic.status();
  }
}