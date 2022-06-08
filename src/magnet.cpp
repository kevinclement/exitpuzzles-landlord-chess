#include "Arduino.h"
#include "magnet.h"
#include "logic.h"

Magnet::Magnet(Logic &logic)
: _logic(logic)
{
}

void Magnet::setup() {
  pinMode(MAGNET_PIN, OUTPUT);
}

void Magnet::handle() {
  digitalWrite(MAGNET_PIN, enabled ? HIGH : LOW);

  if (_prevEnabled != enabled) {
    Serial.print("magnet change: ");
    Serial.println(enabled ? "ENABLED" : "DISABLED");

    _prevEnabled = enabled;
    _logic.status();
  }
}