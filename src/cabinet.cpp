#include "Arduino.h"
#include "cabinet.h"
#include "logic.h"

Cabinet::Cabinet(Logic &logic)
: _logic(logic)
{
}

void Cabinet::setup() {
  pinMode(CABINET_PIN, OUTPUT);
}

void Cabinet::handle() {
  digitalWrite(CABINET_PIN, enabled ? HIGH : LOW);

  if (_prevEnabled != enabled) {
    Serial.print("cabinet change: ");
    Serial.println(enabled ? "ENABLED" : "DISABLED");

    _prevEnabled = enabled;
    _logic.status();
  }
}