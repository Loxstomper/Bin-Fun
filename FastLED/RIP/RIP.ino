//
// Created by Lochie Ashcroft
//

// internal documentation
// ########## NAME ########## // means major section
// ----- NAME ----- // means sub section

// ########## Includes ########## //
// library for controlling the LEDs
#include "FastLED.h"

// libraries for the LCD
#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>

// ########## LCD ########## //
#define I2C_ADDR    0x27
#define BACKLIGHT_PIN     13
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//lcd.begin (16,2); // initialise a 16x2 LCD

// ----- Enable Backlight ----- //
//lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
//lcd.setBacklight(HIGH);
//lcd.home(); // set cursor position to (0, 0)

// initialise the LCD
//LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// ########## MATRIX ########## //
#define LED_PIN 3
#define CHIPSET WS2812B

// datatype for coordinate to led index function
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

// values will be from 0 - 1023
int joy_x;
int joy_y;

// ######### Player ########## //
char direction;
char last_direction;
int score = 0;
int highscore = 0; // stored in ram so every boot it gets lost
int length = 1;
const int max_length = 20;

// ----- Snake Structure ----- //
typedef struct
{
    int x;
    int y;
    int last_x;
    int last_y;
    boolean enabled = false;
} SNAKE;

// initilise player snake
SNAKE body [max_length];

// ######### Pickup ########## //
// coords of the pickup
int pickup_x;
int pickup_y;

// ########## Functions ########## //

// translates coordinate to LED index
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

// gets joystick input and converts to a direction
void get_input()
{
    // Read joystick axis
    joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);

   // used in comparison
   last_direction = direction;
   // values are 0 - 1023 on each axis, using a 100 buffer
    if(joy_x <= 100) // Down
        if(length > 1 && last_direction == 'u')
            direction = 'u';
        else
            direction = 'd';

    if(joy_x >= 923) // Up
        if(length > 1 && last_direction == 'd')
            direction = 'd';
        else
            direction = 'u';

    if(joy_y <= 100) // Left
        if(length > 1 && last_direction == 'r')
            direction = 'r';
        else
            direction = 'l';

    if(joy_y >= 923) // Right
        if(length > 1 && last_direction == 'l')
            direction = 'l';
        else
            direction = 'r';
}

// Spawns the pickup
void spawn_pickup()
{
    // is this needed?
    leds[XY(pickup_x, pickup_y)] = CRGB::Black;

    // HAVE TO CHECK IF THE PIXEL IS OCCUPIED BY THE SNAKE
    // random coord
    pickup_x = random(0, matrix_width);
    pickup_y = random(0, matrix_height);

    // if pickup_x and pickup_y == body[i].x && body[i].y
    // redo

    Serial.print("Spawning Pickup ");
    // enable the LED
    leds[XY(pickup_x, pickup_y)] = CRGB::Blue;
    FastLED.show();
}

void death()
{
    // death animation, light up from head to tail
    Serial.println("Dead");

    // makes entire matrix black
    FastLED.clear();
    FastLED.show();

    // ----- Disables all of the snake elements except the head and resets position ----- //
    for(int i = 1; i < length - 1; i ++)
    {
        body[i].enabled = false;
        body[i].x = 0;
        body[i].y = 0;
        body[i].last_x = 0;
        body[i].last_y = 0;
    }

    // ----- Flashing the screen ----- //
    for(int i = 0; i < NUM_LEDS; i ++)
        leds[i] = CRGB::Red;

    FastLED.show();
    delay(250);
    FastLED.clear();
    FastLED.show();

    for(int i = 0; i < NUM_LEDS; i ++)
        leds[i] = CRGB::Red;

    FastLED.show();
    delay(250);
    FastLED.clear();
    FastLED.show();

    // ----- Reset player ----- //
    // update highscore and display it on the LCD
    if(score > highscore)
    {
        highscore = score;
        //lcd.print("New Highscore!")
        //lcd.setCursor(0, 1);
        //lcd.print(highscore)
    }
    else
    {
        //lcd.print("Highscore: ", highscore);
        //lcd.print("Score        : ", score);
    }

    score = 0; // reset the score value
    length = 1;

    // could do all of this in game start
    // resets player position, bottom right
    body[0].x = 0;
    body[0].y = 0;
    body[0].last_x = 0;
    body[0].last_y = 0;

    // draws the player position
    leds[XY(body[0].x, body[0].y)] = CRGB::Red;
    FastLED.show();

    direction = 'u';
    update_display();
    // wait 1 second to give time for the player
    delay(1000);
}

void update_display()
{
    body[0].last_x = body[0].x;
    body[0].last_y = body[0].y;

    // turn off snake head
    leds[XY(body[0].x, body[0].y)] = CRGB::Black;

    // translate direction into movement, only moving body[0] which is the head
    // make it so you cant go into yourself
    switch(direction)
    {
        case('u'):
            body[0].x ++;
            if(body[0].x > matrix_width - 1)
                body[0].x = 0;
            break;

        case('d'):
            body[0].x --;
            if(body[0].x < 0)
                body[0].x = matrix_width;
            break;

        case('l'):
            body[0].y --;
            if(body[0].y < 0)
                body[0].y = matrix_height;
          break;

        case('r'):
            body[0].y ++;
            if(body[0].y > matrix_height - 1)
                body[0].y = 0;
          break;
    }
    // turn on LED in the new head position
    leds[XY(body[0].x, body[0].y)] = CRGB::Red;
    FastLED.show();

    // check to see if the head is on a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y)
    {
        length ++;
        Serial.print("Length: ");
        Serial.println(length);
        // 0 index - "grows" the snake
        body[length - 1].enabled = true;
        spawn_pickup();
        score += 10; // 10 points for eating a pickup
        //lcd.print("Score :", score);
    }

    // ########## iterates through the snakes body ########## //

    // having an invisible snake the entire time and its components are just enabled
    for(int i = 1; i < max_length - 1; i ++) // CHECK THE LIMITS
    {
        // ----- Updates all snake elements ----- //
        // current position is now the last position
        body[i].last_x = body[i].x;
        body[i].last_y = body[i].y;

        // disable the LED where the element was
        leds[XY(body[i].x, body[i].y)] = CRGB::Black;

        // each element is equal to  the previous element's last position
        body[i].x = body[i-1].last_x;
        body[i].y = body[i-1].last_y;

        // ----- DRAWS ENABLED BODY ELEMENTS ----- //
        if(body[i].enabled == true)
        {
            leds[XY(body[i].x, body[i].y)] = CRGB::Green; // change this
        }
    }

    // updates display for entire snakes position
    FastLED.show();

    // ----- Collision Checking ---- //
    // the only time a Collision will occur is when the head touches another part of the body
    for(int i = 1; i < length - 1; i ++)
    {
        // if the head is in the same location as another enabled element
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
        {
            death();
        }
    }
}

void game_start()
{
    // alter the seed everytime the game gets started
    randomSeed(analogRead(2));
    // fill the screen with random colours
    for(int i = 0; i < NUM_LEDS; i ++)
    {
        leds[i].setRGB( random(0, 256), random(0, 256), random(0, 256));
        FastLED.show();
        delay(25);
    }

    delay(500);
    FastLED.clear();
    // spawn the initial pickup
    spawn_pickup();

    FastLED.show();

    // ----- Set out the lcd layour ----- //
    //lcd.setCursor(0, 1);
    //lcd.print("Score :", score);
    //lcd.setCursor(1, 1):
    //lcd.print("");
    //lcd.setCursor(1, 1):
}

void setup()
{
    // set the led data pin to output
    pinMode(LED_PIN, OUTPUT);

    // LCD
    //lcd.print("> Snake");
    Serial.begin(9600);

    FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    game_start();
}

void loop()
{
    get_input();
    update_display();
    delay(250);
}
