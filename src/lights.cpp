#include "Arduino.h"
#include "lights.h"
#include "logic.h"
#include <FastLED.h>

// Define the array of leds
CRGB leds[NUM_LEDS];

Lights::Lights(Logic &logic)
: _logic(logic)
{
}

void Lights::setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // for(int i=0;i<NUM_LEDS; i++) {
  //   leds[i] = CRGB::Blue;
  // }
}

void Lights::handle() {
  // FastLED.show();
  // FastLED.delay(1000/FRAMES_PER_SECOND);

  // leds[0] = CRGB::Red;
  // FastLED.show();
  // delay(1000);

  // leds[1] = CRGB::Red;
  // FastLED.show();
  // delay(1000);

  // leds[2] = CRGB::Red;
  // FastLED.show();
  // delay(1000);

  // // Now turn the LED off, then pause
  // leds[0] = CRGB::Black;
  // leds[1] = CRGB::Black;
  // leds[2] = CRGB::Black;
  // FastLED.show();
  // delay(3000); 
}