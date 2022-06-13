#include "Arduino.h"
#include "logic.h"
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27

MelodyPlayer player1(BUZZER_PIN_1, HIGH);
MelodyPlayer player2(BUZZER_PIN_2, HIGH);

const char bustMelodyStr[] = "bust:d=16,o=5,b=112:32p,f,a#,c6,c#6,c6,c#6,d#6,2f6";
const char solvedMelodyStr[] = "is:d=8,o=5,b=160:4p,d6,c6,4d6,f6,4a#6,4a6,2c7";

// leisure larry
// const char solvedMelodyStr[] = "solved:d=16,o=6,b=56:g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.";

// Itchy and Scratchy
// const char solvedMelodyStr[] = "is:d=8,o=5,b=160:4p,d6,c6,4d6,f6,4a#6,4a6,2c7";

Melody bustMelody;
Melody solvedMelody;
Sound::Sound(Logic &logic)
: _logic(logic)
{
}

void Sound::setup() {
  bustMelody = MelodyFactory.loadRtttlString(bustMelodyStr);
  solvedMelody = MelodyFactory.loadRtttlString(solvedMelodyStr);
}

void Sound::handle() {
}

void Sound::bustTriggered() {
  player1.playAsync(bustMelody);
  player2.playAsync(bustMelody);
}

void Sound::solved() {
  player1.playAsync(solvedMelody);
  player2.playAsync(solvedMelody);
}