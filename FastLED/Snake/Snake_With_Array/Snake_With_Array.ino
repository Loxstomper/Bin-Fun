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
int length;
const int max_length = 20;

// ----- Snake Structure ----- //
typedef struct
{
    int x;
    int y;
    boolean enabled = false;
} SNAKE;

// initilise player snake
SNAKE body [max_length];

// ######### Pickups ########## //
boolean pickup_exists;
int pickup_x;
int pickup_y;

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

// function to fill screen passing the colour variable
//
//

void get_input()
{
    // values are 0 - 1023 on each axis, using a 100 buffer
    joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);

    // Down
    if(joy_x <= 100)
        direction = 'd';
    // Up
    if(joy_x >= 923)
        direction = 'u';
    // Left
    if(joy_y <= 100)
        direction = 'l';
    // Right
    if(joy_y >= 923)
        direction = 'r';

    update_display();
}

void pickup()
{
    // spawn pickup if one does not exist
    if(pickup_exists == false)
    {
        pickup_x = random(0, matrix_width + 1);
        pickup_y = random(0, matrix_height + 1);

        leds[XY(pickup_x, pickup_y)] = CRGB::Amethyst;
        matrix.drawPixel(pickup_x, pickup_y, matrix.Color333(BLUE));
        pickup_exists = true;
    }
}

void death()
{
    // ----- Lights up snake from head to tail ----- //
    for(int i = 0; i < length; i ++)
    {
        leds[XY(body[i].x, body[i].y)] = CRGB::Red;
        FastLED.show();
        delay(100);
    }
    delay(500);

    // clears led data - colour
    FastLED.clear();
    // may need a delay
    FastLED.show();

    // ----- Disables all of the snake except the head ----- //
    for(int i = 1; i < length - 1; i ++) // check limit
        body[i].enabled = false;

    // ----- Flashing the screen ----- //
    for(int i = 0; i < NUM_LEDS; i ++)
        leds[i] = CRGB::Red;

    FastLED.show();
    delay(250);
    FastLED.clear();
    // may need a delay
    FastLED.show();

    for(int i = 0; i < NUM_LEDS; i ++)
        leds[i] = CRGB::Red;

    FastLED.show();
    delay(250);
    FastLED.clear();
    // may need a delay
    FastLED.show()

    // ----- Reset player ----- //
    score = 0;
    length = 1;

    // resets player position, bottom left
    body[0].x = 0;
    body[0].y = 0;
    // draws the player position
    leds[XY(body[0].x, body[0].y)] = CRGB::Aquamarine
    FastLED.show();

    direction = 'u';
    update_display();
    // wait 1 second to give time for the player
    delay(1000);
    // make pickups spawnable
    pickup_exists = false;
}

void update_display()
{
    // turn off snake head
    leds[XY(body[0].x, body[0].y)] = CRGB::Black;

    // translate direction into movement, only moving body[0] which is the head
    switch(direction)
    {
        case('u'):
            //Serial.println("Up");
            body[0].x ++;
            // if hit at the top of the matrix go to the bottom row
            if(body[0].x > matrix_height)
                body[0].x = 0;
            break;

        case('d'):
            //Serial.println("Down");
            body[0].x --;
            // if hit at the bottom of the matrix go to top row
            if(body[0].x < 0)
                body[0].x = matrix_height;
            break;

        case('l'):
            //Serial.println("Left");
            body[0].y --;
            // if hit at the left of the matrix go to right column
            if(body[0].y < 0)
                body[0].y = matrix_width;
          break;

        case('r'):
            //Serial.println("Right");
            body[0].y ++;
            // if hit at the right of the matrix go to left column
            if(body[0].y > matrix_width)
                body[0].y = 0;
          break;
    }
    // turn on LED in the new head position
    leds[XY(body[0].x, body[0].y)] = CRGB::Aquamarine;
    // DECIDE IF I NEED TO SHOW HERE
    FastLED.show();

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

void game_start()
{
    for(i = 0; i < NUM_LEDS; i ++)
    {
        leds[i],setRGB( random(0, 256), random(0, 256), random(0, 256));
        FastLED.show();
        delay(50);
    }
}



void setup()
{
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);

  game_start();
  FastLED.clear();
}

void loop()
{
    input();
    movement();
    delay(100);
}
