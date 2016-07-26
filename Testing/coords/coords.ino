#include "FastLED.h"

#define LED_PIN  3
#define CHIPSET     WS2812B

// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 10;
// series layout
const bool    kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if( y & 0x01)
  {
    // Odd rows run backwards
    uint8_t reverseX = (kMatrixWidth - 1) - x;
    i = (y * kMatrixWidth) + reverseX;
  }
  else
  {
    // Even rows run forwards
    i = (y * kMatrixWidth) + x;
  }
  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0)); // sets up random seed using the input of an unconnected pin as the seed
  FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
    // need to store these values
    int x = random(0, kMatrixWidth + 1);
    int y = random(0, kMatrixHeight + 1);
    // random led gets random colour
    leds[XY(x, y)] == CRGB(random(0, 256), random(0, 256), random(0, 256));
    FastLED.show();
    delay(random(50, 1001));
    // disables the led
    leds[XY(x, y)] = CRGB::Black;
}
