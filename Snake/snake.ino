#include "FastLED.h"

#define LED_PIN  3
#define CHIPSET     WS2812B

// Matrix dimensions
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 10;

// Joystick
const int joy_x_pin;
const int joy_y_pin;
int joy_x;
int joy_y;

// Snake
int speed = 1;
int length = 1;
// head pos
int snake_x = 0;
int snake_y = 0;

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

void movement()
{
    // (0,0) is bottom right

    // values are 0 - 1023 on each axis
    joy_x_status = digitalRead(joy_x_pin);
    joy_y_status = digitalRead(joy_y_pin);

    // if(joy_x_status == 0 && snake_x < 7)
    // {
    //     // MOVE LEFT
    // }
    // if(joy_x_status == 1023 && snake_x > 0)
    // {
    //     // MOVE RIGHT
    // }
    // if(joy_y_status == 0 && snake_y > 0)
    // {
    //     // MOVE DOWN
    // }
    // if(joy_y_status == 1023 && snake_y < 9)
    // {
    //     // MOVE UP
    // }

    // TESTING SNAKE IS size 1, movement in blocked when hitting a wall in that direction
    // LED stuff
    leds[XY(x, y)] == CRGB::Black; // turn off previous LED

    if(joy_x_status == 0 && snake_x < 7)
    {
        snake_x ++;
    }
    if(joy_x_status == 1023 && snake_x > 0)
    {
        snake_x --;
    }
    if(joy_y_status == 0 && snake_y > 0)
    {
        snake_y --;
    }
    if(joy_y_status == 1023 && snake_y < 9)
    {
        snake_y ++;
    }

    leds[XY(x, y)] == CRGB::Red;

}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0)); // sets up random seed using the input of an unconnected pin as the seed
  FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{

}
