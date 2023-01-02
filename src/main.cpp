#include <Arduino.h>
#include "logic.h"

Logic logic;

boolean checkTimePeriod (unsigned long &expireTime, unsigned long TimePeriod) {
  unsigned long currentMillis  = millis();
  if ( currentMillis - expireTime >= TimePeriod ) {
    expireTime = currentMillis;
    return true;
  }
  else return false;
}

void heartBeatBlink() {
  static unsigned long blinkTimer;
  pinMode(LED_BUILTIN, OUTPUT);

  if (checkTimePeriod(blinkTimer, 100)) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN) );
  }
}

void debug() {
  Serial.println("toggling debug of device...");
  logic.debug = !logic.debug;
}

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  // read and handle message from serial
  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  if (msg == "reset" || msg == "reboot" || msg == "r") {
    ESP.restart();
  } else if (msg == "attract" || msg == "a") {    
    logic.lights.triggerAttractMode();
  } else if (msg == "solve1" || msg == "1") {
    logic.triggerFirst();
  } else if (msg == "solve2" || msg == "2") {
    logic.triggerSecond();
  } else if (msg == "status" || msg == "s") {
    logic.status();
  } else if (msg == "blinkOn" || msg == "b") {
    logic.cabinetLed.blinkOn();
  } else if (msg == "blinkOff" || msg == "n") {
    logic.cabinetLed.blinkOff();
  } else if (msg == "sound" || msg == "o") {
    logic.sound.bustTriggered();
  } else if (msg == "doorOn" || msg == "d") {
    logic.magnet.enabled = true;
  } else if (msg == "doorOff" || msg == "f") {
    logic.magnet.enabled = false;
  } else if (msg == "cabinetOn" || msg == "c") {
    logic.cabinet.enabled = true;
  } else if (msg == "cabinetOff" || msg == "v") {
    logic.cabinet.enabled = false;
  } else if (msg == "cabinetLedOff" || msg == "z") {
    logic.cabinetLed.enabled = false;
  } else if (msg == "speakerLedOff" || msg == "o") {
    logic.speakerLed.enabled = false;
  } else if (msg == "h") {
    Serial.println("turning lights off");
    lights.triggerLightsOff();
  } else if (msg == "debug" || msg == "x") {
    debug();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  Serial.println("READY");
  Serial.println("Chess Board by kevinc...\n");

  logic.setup();
  logic.status();
}

void loop() {
  heartBeatBlink();
  readAnySerialMessage();
  logic.handle();
}
