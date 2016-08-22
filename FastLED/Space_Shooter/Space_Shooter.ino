#include "FastLED.h"

// ########## MATRIX ########## //
#define LED_PIN 3
#define CHIPSET WS2812B

// datatype for coord to index
const uint8_t kMatrixWidth = 9;
const uint8_t kMatrixHeight = 12;

// variables for logic
const int matrix_width = 9;
const int matrix_height = 12;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1); // pointer

// ######### Joystick ########## //
const int joy_x_pin = A0;
const int joy_y_pin = A1;
// 0 - 1023
int joy_x;
int joy_y;

// ######### Player ########## //
char direction;
int score;

typedef struct
{
    int x;
    int y;
} SHIP;

SHIP player [3];

// ########## Functions ########## //

// translates coord to index
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

void get_input()
{
    // values are 0 - 1023 on each axis, using a 100 buffer
    //joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);

    // Down
    //if(joy_x <= 100)
    //    direction = 'd';
    // Up
    //if(joy_x >= 923)
    //    direction = 'u';
    // Left
    if(joy_y <= 100)
        direction = 'l';
    // Right
    if(joy_y >= 923)
        direction = 'r';

    update_display();
}

void update_display()
{
    switch(direction)
    {
        /*case('u'):
            if(player[1].y < matrix_height - 1)
            {
                for(int i = 0; i < 2; i ++)
                {
                    leds[XY(player[i].x, player[i].y)] = CRGB::Black;
                    player[i].y ++;
                    leds[XY(player[i].x, player[i].y)] = CRGB::Red;
                }
                FastLED.show();
            }
        break;*/

        //case('d'):
        //break;

        case('l'):
            if(player[0].x < matrix_width - 1)
            {
                for(int i = 0; i < 3; i ++)
                {
                    leds[XY(player[i]).x, player[i].y)] = CRGB::Black;
                    player[i].x ++;
                    leds[XY(player[i]).x, player[i].y)] = CRGB::Red;
                }
                FastLED.show();
                direction = 's' // stop
            }
          break;

        case('r'):
        if(player[3].x > 0)
        {
            for(int i = 0; i < 3; i ++)
            {
                leds[XY(player[i]).x, player[i].y)] = CRGB::Black;
                player[i].x --;
                leds[XY(player[i]).x, player[i].y)] = CRGB::Red;
            }
            FastLED.show();
            direction = 's' // stop
        }
          break;
    }

    // if the head is on a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y && pickup_exists == true)
    {
        //Serial.println("On a pickup");
        length ++;
        // 0 index - "grows" the snake
        body[length - 1].enabled = true;
        // make pickups spawnable
        pickup_exists == false;
        // add 10 to the score for picking up
        score += 10;
    }

    // ########## iterates through the snakes body ########## //
    //maybe issue with element 1 and element 0, if there is, make element 1 = element 0
    //before element 0 is modified and remeove element 1 from for loop

    // having an invisible snake the entire time and its components are just enabled
    for(int i = 1; i < max_length - 1; i ++) // CHECK THE LIMITS
    {
        // ----- UPDATES POSITION ----- //
        body[i].x = body[i-1].x;
        body[i].y = body[i-1].y;

        // ----- DRAWS ENABLED BODY ELEMENTS ----- //
        if(i <= length - 1 && body[i] == false) // saves enabling elements that allready are enabled
        {
            body[i].enabled = true;
            leds[XY(body[i].x, body[i].y)] = CRGB::Aquamarine;
        }
    }

    // updates display for entire snakes position
    FastLED.show();

    // ----- Collision Checking ---- //
    // the only time a Collision will occur is when the head touches another part of the body
    for(int i = 1; i < length - 1; i ++) // check the LIMITS
    {
        //Serial.println("Snake Collided");
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
        {
            //Serial.println("Snake Collided");
            death();
        }
    }
}
