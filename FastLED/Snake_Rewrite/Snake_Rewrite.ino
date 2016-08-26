// Created by Lochie Ashcroft //

// internal documentation
// ########## NAME ########## // is a major section
// ----- NAME ----- // is a sub section

// ########## Libraries ########## //
// library for controlling the LEDs
#include "FastLED.h"

// libraries for the LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

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

// initialise the LCD
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// ########## MATRIX ########## //
#define LED_PIN 3
#define CHIPSET WS2812B

// datatype for coordinate to led index function - unisgned int
const uint8_t kMatrixWidth = 9;
const uint8_t kMatrixHeight = 12;

// variables for logic
const int matrix_width = 9;
const int matrix_height = 12;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
CRGB* const leds(leds_plus_safety_pixel + 1); // pointer

// ######### Input ########## //

// ----- Joystick ----- //
// analog pins for the joystick
const int joy_x_pin = A0;
const int joy_y_pin = A1;

// values will be from 0 - 1023
int joy_x;
int joy_y;

// ----- Button ----- //
const int button_pin = 2;


// ######### Player ########## //
char direction;
char last_direction;
int score = 0;
int length = 1;
const int max_length = 20;

// ----- Snake Structure ----- //
typedef struct
{
    // setting to 0 to make the code neater in snake_start()
    int x = 0;
    int y = 0;
    int last_x = 0;
    int last_y = 0;
    boolean enabled = false;
} SNAKE;

// initialises body as the snake datatype
SNAKE body [max_length];

// ######### Game ######### //

int speed = 500; // delay in ms before input, can be altered
int highscore = 0; // stored in ram so every boot it gets lost
boolean playing = false; // is the game running?

// ######### Pickup ########## //

// coords of the pickup
int pickup_x;
int pickup_y;

// ########## Functions ########## //

uint16_t XY(uint8_t x, uint8_t y)
{
    // translates coordinate to LED index
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
    // Reads the joystick axis values ranging from 0-1023
    joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);
}

void input_to_direction()
{
    // translates the joystick axis values to a direction

    last_direction = direction; // used to stop moving in the previous direction when larger than 1 unit

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

void spawn_pickup()
{
    // spawns a pickup in a random location in which the snake is not occupying

    // random coords for the pickup
    pickup_x = random(0, matrix_width);
    pickup_y = random(0, matrix_height);

    // validating that the random coords are not occuipied by the snake
    for(int i = 0; i < length - 1; i ++)
    {
        // not a valid position, redo random coords
        if(pickup_x == body[i].x && pickup_y == body[i].y && body[i].enabled == true)
            spawn_pickup();
    }

    // enable the LED
    leds[XY(pickup_x, pickup_y)] = CRGB::Blue;
    FastLED.show();
}

void death_animation()
{
    // lights from head to tail (point of collision)
    for(int i = 0; i < length - 1; i ++)
    {
        LEDS[XY(body[i].x, body[i].y)] = CRGB::Red;
        FastLED.show();
        delay(100);
    }
}

void update_LCD()
{
    // prints the existing highscore or the new one if applicable

    if(score > highscore)
    {
        highscore = score; // update highscore

        // update LCD
        lcd.home(); // sets cursor position to 0,0
        lcd.print("New Highscore!")
        lcd.setCursor(0, 1);
        lcd.print(highscore)
    }
    else
    {
        // highscore was not beaten
        lcd.home();
        lcd.print("Highscore: " + highscore);
        lcd.setCursor(0, 1);
        lcd.print("Score        : " + score);
    }
}

void reset_player()
{
    // resets all player variables

    score = 0;
    length = 1;

    // resets body object
    for(int i = 0; i < length - 1; i ++)
    {
        body[i].x = 0;
        body[i].y = 0;
        body[i].last_x = 0;
        body[i].last_y = 0;

        // compacting code - the head is allways enabled, and its initial position is 0,0
        if(i != 0)
            body[i].enabled = false;
    }

    direction = 'u'; // on game start the player will go up
}

void death()
{
    // calls the death sequence

    // LCD says game over
    lcd.home();
    lcd.print("Game Over!")
    delay(250);

    death_animation();
    update_LCD();
    reset_player();

    // makes entire matrix black
    FastLED.clear();
    FastLED.show();

    playing = false;
}

void update_snake()
{
    // updates the head position then the rest of the snake

    // last position is now the current position
    body[0].last_x = body[0].x;
    body[0].last_y = body[0].y;

    // turn off snake head
    leds[XY(body[0].x, body[0].y)] = CRGB::Black;

    // Updating head position
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

    // enable the LED in the position of the head
    leds[XY(body[0].x, body[0].y)] = CRGB::Red;

    // updating rest of the body
    for(int i = 1; i < max_length - 1; i ++)
    {
        // current position is now the last position
        body[i].last_x = body[i].x;
        body[i].last_y = body[i].y;

        // disable the LED where the element was
        leds[XY(body[i].x, body[i].y)] = CRGB::Black;

        // each element is equal to  the previous element's last position
        body[i].x = body[i-1].last_x;
        body[i].y = body[i-1].last_y;

        // enable the LED in the position of the enabled body elements
        if(body[i].enabled == true)
        {
            leds[XY(body[i].x, body[i].y)] = CRGB::Green; // change this
        }
    }

    // update the LEDs
    fastLED.show();
}

void collision_check()
{
    // checks if the snake has collided with anything

    // collision with a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y)
    {
        length ++;
        score += 10;
        // 0 index - "grows" the snake
        body[length - 1].enabled = true;

        // update the LCD
        lcd.home();
        lcd.print("Score: " + score);

        spawn_pickup(); // spawn another pickup
    }

    // collision with itself
    for(int i = 1; i < length - 1; i ++)
    {
        // if the head is in the same location as another enabled element
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
            death();
    }
}

void snake_start()
{
    // starts the snake game

    // Change the random seed every game using the input of an unconnected analog pin
    randomSeed(analogRead(A2));

    // splash screen
    for(int i = 0; i < NUM_LEDS; i ++)
    {
        leds[i].setRGB( random(0, 256), random(0, 256), random(0, 256));
        FastLED.show();
        delay(25);
    }

    delay(250);

    // clear the screen
    FastLED.clear();
    FastLED.show();

    // ----- Set out the lcd layout ----- //
    lcd.home();
    lcd.print("Score: " + score);
    //lcd.setCursor(1, 1):
    //lcd.print("");
    //lcd.setCursor(1, 1):
}

void idle()
{
	// function when the game is not playing, animation, advertising etc

    FastLED.clear();
	// just making the entire display a random colour and brighting / dimming
	fill_solid(leds, NUM_LEDS, CRGB(random(0, 256), random(0, 256), random(0, 256)));
    //http://fastled.io/docs/3.1/group___dimming.html#gae51352a3522bb8eae5e17dbee97aa1aa
    FastLED.show();
    delay(1000);

}

void setup()
{
    // set the led data pin to output
    pinMode(LED_PIN, OUTPUT);

    // joystick pins to input
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);

    // button to input
    pinMode(button_pin, INPUT);

    // LCD
    lcd.begin (16,2); // initialise a 16x2 LCD
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home(); // set cursor position to (0, 0)
    lcd.print("> Snake");
    // maybe to a howto? in the menu

    // create the LED matrix
    FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
    // clearing just incase
    FastLED.clear();
    FastLED.show();
}

void loop()
{
    while (playing == false);
    {
        // screensaver
        idle();
        // menu system

        if(digitalRead(button_pin) == HIGH)
            playing = true;
    }

    // time to play snake
    snake_start();

    // while the game is being played, play
    while (playing)
    {
        get_input();
        input_to_direction();
        update_snake();
        collision_check();
        delay(speed);
    }
}
