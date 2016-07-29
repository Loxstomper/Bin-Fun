// defines up here and libraries
#include "FastLED.h"

#define NUM_LEDS 80 // number of leds in the matrix
#define DATA_PIN 3

CRGB leds[NUM_LEDS]; // led array

void setup()
{
	pinMode(DATA_PIN, OUTPUT);
	FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
	// up
	for(int i = 0; i < NUM_LEDS; i ++)
	{
		leds[i] = CRGB::Red;
		FastLED.show();
		delay(200);
		leds[i] = CRGB::Black;
		FastLED.show();

		leds[i] = CRGB::Green;
		FastLED.show();
		delay(200);
		leds[i] = CRGB::Black;
		FastLED.show();

		leds[i] = CRGB::Blue;
		FastLED.show();
		delay(200);
		leds[i] = CRGB::Black;
		FastLED.show();
	}

	// down
	for(int i = NUM_LEDS; i > 0; i --)
	{
		// make the led red
		leds[i] = CRGB::Blue;
		FastLED.show();
		delay(200);
		leds[i] = CRGB::Black;
	}

	// up
	for(int i = 0; i < NUM_LEDS; i ++)
	{
		leds[i] = CRGB(random(0, 256), random(0, 256), random(0, 256));
		FastLED.show();
		delay(200);
		leds[i] = CRGB::Black;
	}
 }
