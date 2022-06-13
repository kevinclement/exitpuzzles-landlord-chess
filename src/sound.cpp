#include "Arduino.h"
#include "logic.h"
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27

MelodyPlayer player1(BUZZER_PIN_1, HIGH);
MelodyPlayer player2(BUZZER_PIN_2, HIGH);

const char bustMelodyStr[] = "bust:d=16,o=5,b=112:32p,f,a#,c6,c#6,c6,c#6,d#6,2f6";
const char solvedMelodyStr[] = "SecretOf:d=4,o=5,b=100:16p,16g,16c6,16d6,8d#.6,8d.6,8c6,16p,16g,16c6,16d6,g6,8p";

// Leisure Larry: "solved:d=16,o=6,b=56:g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#."
// Itchy and Scratchy: "is:d=8,o=5,b=160:4p,d6,c6,4d6,f6,4a#6,4a6,2c7";
// Pingu: "pingu:o=6,d=4,b=285:f,g,p,f,e,p,g,p,c7,c";
// Burgertime: "burger:d=4,o=6,b=285:8e7,8c#7,8c7,8a#,8g#,8g,8g#,8g,g#,c#7,g#,c#7,g#,c#7";
// Hamster: "HamsterD:d=16,o=6,b=140:e7,c#7,b,c#7,b,g#,b,g#,f#,e,c#,e,f#,e,g#,b,g#,b,f#,e,8c#,2e"
// Monkey Island: "SecretOf:d=4,o=5,b=100:16p,16g,16c6,16d6,8d#.6,8d.6,8c6,16p,16g,16c6,16d6,g6,8p"
// Final Fantasy: "FinalFan:d=4,o=5,b=112:16a4,8c.,16a4,16a4,16a4,8d.,16a4,16a4,16a4,16d#,16d,16c,16d,16c,16b4,8c,16b4,16c";

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