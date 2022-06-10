#include "Arduino.h"
#include "pn532Device.h"
#include "logic.h"

PN532Device::PN532Device(Logic &logic)
: _logic(logic)
{
}

void PN532Device::setup() {
}

void PN532Device::handle() {
}