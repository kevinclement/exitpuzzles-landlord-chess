#include "Arduino.h"
#include "logic.h"
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27

MelodyPlayer player1(BUZZER_PIN_1, HIGH);
MelodyPlayer player2(BUZZER_PIN_2, HIGH);

const char bustMelodyStr[] = "bust:d=16,o=5,b=112:32p,f,a#,c6,c#6,c6,c#6,d#6,2f6";

Melody bustMelody;
Sound::Sound(Logic &logic)
: _logic(logic)
{
}

void Sound::setup() {
  bustMelody = MelodyFactory.loadRtttlString(bustMelodyStr);
}

void Sound::handle() {
}

void Sound::bustTriggered() {
  player1.playAsync(bustMelody);
  player2.playAsync(bustMelody);
}

void Sound::solved() {
}