#include "FastLED.h"

#define LED_PIN  3
#define CHIPSET  WS2812B

// Matrix dimensions
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 10;
const int matrix_width = 8;
const int matrix_height = 10;

// Joystick
const int joy_x_pin = A0;
const int joy_y_pin = A1;
int joy_x;
int joy_y;

// Snake
int speed = 1;
int length = 1;
char direction = 'u'; // switch case must have data type that is enumerable i.e a char instead of a string
// head pos
int snake_x = 0;
int snake_y = 0;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1); // pointer

// XY Function
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if( y & 0x01)
  {
    uint8_t reverseX = (kMatrixWidth - 1) - x;
    i = (y * kMatrixWidth) + reverseX;
  }
  else
  {
    i = (y * kMatrixWidth) + x;
  }
  return i;
}

void input()
{
    // values are 0 - 1023 on each axis
    joy_x = digitalRead(joy_x_pin);
    joy_y = digitalRead(joy_y_pin);

    if(joy_x == 0) // LEFT
    {
        direction = 'l';
    }
    if(joy_x == 1023) // RIGHT
    {
        direction = 'r';
    }
    if(joy_y == 0) // DOWN
    {
        direction = 'd';
    }
    if(joy_y == 1023) // UP
    {
        direction = 'u';
    }
}

void movement()
{
    // 0,0 is bottom right hence why x is being increased to move left

    leds[XY(snake_x, snake_y)] == CRGB::Black; // turn off current LED
    switch( direction)
    {
        case( 'u' ):
            if( snake_y < (matrix_height - 1) )
            {
                snake_y ++;
            }
            // else die animation or whatever
            break;

        case( 'd' ):
            if( snake_y > 0 )
            {
                snake_y --;
            }
            // else die animation or whatever
            break;

        case( 'l' ):
            if( snake_x < (matrix_width - 1) )
            {
                snake_x ++;
            }
            // else die animation or whatever
            break;

        case( 'l' ):
            if( snake_x > 0 )
            {
                snake_x --;
            }
            // else die animation or whatever
            break;
    }

    leds[XY(snake_x, snake_y)] == CRGB::Red; // new position
    FastLED.show();
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
    input();
    delay(250 / speed);
}
