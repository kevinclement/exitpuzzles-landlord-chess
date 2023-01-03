#include <Arduino.h>
#include "logic.h"

Logic logic;

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
    logic.speakerLed.blinkOn();
  } else if (msg == "blinkOff" || msg == "n") {
    logic.speakerLed.blinkOff();
  } else if (msg == "soundBust" || msg == "o") {
    logic.sound.bustTriggered();
  } else if (msg == "soundBoard" || msg == "p") {
    logic.sound.solved();
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
  } else if (msg == "finish" || msg == "i") {
    logic.finish();
  } else if (msg == "lightsOff" || msg == "h") {
    Serial.println("Turning all lights off");
    logic.lights.triggerLightsOff();
  } else if (msg == "piecesOff" || msg == "k") {
    Serial.println("Disabling chess pieces...");
    logic.togglePiecesDisabled(true);
  } else if (msg == "piecesOn" || msg == "l") {
    Serial.println("Enabling chess pieces...");
    logic.togglePiecesDisabled(false);
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
  readAnySerialMessage();
  logic.handle();
}
