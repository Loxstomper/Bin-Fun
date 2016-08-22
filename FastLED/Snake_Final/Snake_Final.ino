//
//  Info about the sketch
//

// ########## NAME ########## // means major section
// ----- NAME ----- // means sub section

// ########## Includes ########## //
#include "FastLED.h" // library for controlling the LEDs

// libraries for the LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// ########## LCD ########## //
#define I2C_ADDR    0x27
#define BACKLIGHT_PIN     13
#define En_pin  2
#define Rw_pin  1lcd.begin (16,2);
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

// ----- Enable Backlight ----- //
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home(); // set cursor position to (0, 0)

// initilise the LCD
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

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
// values will be from 0 - 1023
int joy_x;
int joy_y;

// ######### Player ########## //
char direction;
int score = 0;
int highscore = 0; // stored in ram so every boot it gets lost
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

// ######### Pickup ########## //
// coords of the pickup
int pickup_x;
int pickup_y;

// ########## Functions ########## //

// translates coord to index of the LED
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

   // values are 0 - 1023 on each axis, using a 100 buffer
    if(joy_x <= 100) // Down
        direction = 'd';

    if(joy_x >= 923) // Up
        direction = 'u';

    if(joy_y <= 100) // Left
        direction = 'l';

    if(joy_y >= 923) // Right
        direction = 'r';
}

// Spawns the pickup
void pickup()
{
    // disable the previous location
    // not sure if it this will work because in the begining these variables have no value
    // can do an if statement to check values are greater than 0
    leds[XY(pickup_x, pickup_y)] = CRGB::Black;

    // random coord
    pickup_x = random(0, matrix_width);
    pickup_y = random(0, matrix_height);

    // enable the LED
    leds[XY(pickup_x, pickup_y)] = CRGB::Amethyst;
    FastLED.show();
}

// Function that is called when the snake dies
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

    // makes entire matrix black
    FastLED.clear();
    FastLED.show();

    // ----- Disables all of the snake elements except the head ----- //
    for(int i = 1; i < length - 1; i ++) // check limit
        body[i].enabled = false;

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
        lcd.print("New Highscore!")
        lcd.setCursor(0, 1);
        lcd.print(highscore)
    }
    else()
    {
        lcd.print("Highscore: ", highscore);
        lcd.print("Score        : ", score);
    }

    score = 0; // reset the score value
    length = 1;

    // could do all of this in game start
    // resets player position, bottom right
    body[0].x = 0;
    body[0].y = 0;

    // draws the player position
    leds[XY(body[0].x, body[0].y)] = CRGB::Aquamarine;
    FastLED.show();

    direction = 'u';
    update_display();
    // wait 1 second to give time for the player
    delay(1000);
}

void update_display()
{
    // turn off snake head
    leds[XY(body[0].x, body[0].y)] = CRGB::Black;

    // translate direction into movement, only moving body[0] which is the head
    switch(direction)
    {
        case('u'):
            body[0].x ++;
            // if hit at the top of the matrix go to the bottom row
            if(body[0].x > matrix_height)
                body[0].x = 0;
            break;

        case('d'):
            body[0].x --;
            // if hit at the bottom of the matrix go to top row
            if(body[0].x < 0)
                body[0].x = matrix_height;
            break;

        case('l'):
            body[0].y --;
            // if hit at the left of the matrix go to right column
            if(body[0].y < 0)
                body[0].y = matrix_width;
          break;

        case('r'):
            body[0].y ++;
            // if hit at the right of the matrix go to left column
            if(body[0].y > matrix_width)
                body[0].y = 0;
          break;
    }
    // turn on LED in the new head position
    leds[XY(body[0].x, body[0].y)] = CRGB::Aquamarine;
    FastLED.show();

    // check to see if the head is on a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y)
    {
        length ++;
        // 0 index - "grows" the snake
        body[length - 1].enabled = true;
        spawn_pickup();
        score += 10; // 10 points for eating a pickup
        lcd.print("Score :", score);
    }

    // ########## iterates through the snakes body ########## //
    //maybe issue with element 1 and element 0, if there is, make element 1 = element 0
    //before element 0 is modified and remeove element 1 from for loop

    // having an invisible snake the entire time and its components are just enabled
    for(int i = 1; i < max_length - 1; i ++) // CHECK THE LIMITS
    {
        // CHECK THAT THIS DOESNT JUST STACK ALL OF THE ELEMENTS AT 1 LOCATION
        // ----- Updates all snake elements ----- //
        // each element goes to where the previous element was previously
        body[i].x = body[i-1].x;
        body[i].y = body[i-1].y;

        // ----- DRAWS ENABLED BODY ELEMENTS ----- //
        if(i <= length - 1 && body[i].enabled == false) // saves enabling elements that already are enabled
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
        // if the head is in the same location as another enabled element
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
        {
            death();
        }
    }
}

void game_start()
{
    // fill the screen with random colours
    for(int i = 0; i < NUM_LEDS; i ++)
    {
        leds[i].setRGB( random(0, 256), random(0, 256), random(0, 256));
        FastLED.show();
        delay(25);
    }

    lcd.setCursor(0, 1);
    lcd.print("Score :", score);
    lcd.setCursor(1, 1):
    lcd.print("");
    lcd.setCursor(1, 1):
}

void setup()
{
    // LCD
    lcd.print("> Snake");

    pinMode(LED_PIN, OUTPUT);
    FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    game_start();
}

void loop()
{
    get_input();
    update_display();
    delay(500);
}
