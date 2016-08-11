#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

// ########## MATRIX ########## //
#define CLK 8
#define LAT A3
#define OE 9
#define A A0
#define B A1
#define C A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
const int matrix_width = 16; // inverted axis due to using portrait orientation
const int matrix_height = 32; // inverted axis due to using portrait orientation

// ----- Colours ----- //
#define BLACK 0, 0, 0
#define RED 7, 0, 0
#define GREEN 0, 7, 0
#define BLUE 0, 0, 7
// maybe make a library of colours so to access it would be COLOUR.blue, etc.

// ########## Joystick ##########//
const int joy_x_pin = A4;
const int joy_y_pin = A5;
int joy_x; // 0-1023
int joy_y; // 0-1023

// ########## Player ########## //
int x = 0;
int y = 16;
char direction = 'u';
int score;

// ########## Pickups i.e. fruit ########## //
boolean pickup_exists;
int pickup_x;
int pickup_y;

// ########## Functions ########## //
void get_input()
{
    // values are 0 - 1023 on each axis, using a 100 buffer
    joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);

    if(joy_x <= 100) // Down
        direction = 'd';
    if(joy_x >= 923) // Up
        direction = 'u';
    if(joy_y <= 100) // Left
        direction = 'l';
    if(joy_y >= 923) // Right
        direction = 'r';

    update_display();
}

void update_display()
{
    // disable player led
    matrix.drawPixel(x, y, matrix.Color333(BLACK));

    // translate direction into movement
    switch(direction)
    {
        case('u'):
            x ++;
            if(x > matrix_height) // if hit the top go to botton
                x = 0;
            break;

        case('d'):
            x --;
            if(x < 0) // if hit the bottom go to top
                x = 32;
            break;

        case('l'):
            y --;
            if(y < 0) // if hit the left go to right
                y = 16;
          break;

        case('r'):
            y ++;
            if(y > 16) // if hit the right go to right
                y = 0;
          break;
    }
    // turn on LED in the new position
    matrix.drawPixel(x, y, matrix.Color333(GREEN));

    // check if on top of pickup - delay is minimal so it will look ok
    if(x == pickup_x && y == pickup_y && pickup_exists == true) // needs pickup_exists otherwise will change colour on the coord
    {
        // grow snake or whatever, ill just change color when it picks up the pickup
        matrix.drawPixel(x, y, matrix.Color333(random(0, 8), random(0, 8), random(0, 8)));
        // "remove" pickup - stops from spawning more
        pickup_exists == false;
        score += 10; // add 10 to the score for picking up
    }
}

void death()
{
    // disable player led
    matrix.drawPixel(x, y, matrix.Color333(BLACK));

    // Flash screen signalling death
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    matrix.fillScreen(matrix.Color333(BLACK));
    delay(250);
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    matrix.fillScreen(matrix.Color333(BLACK));
    delay(250);

    // reset player
    score = 0;
    x = 0;
    y = 0;
    matrix.drawPixel(x, y, matrix.Color333(GREEN));
    direction = 'u';

    update_display();
    delay(1000); // wait 1 second to give time for the player
    pickup_exists = false;
}

void pickup()
{
    // spawn pickup if one does not exist
    if(pickup_exists == false)
    {
        pickup_x = random(0, 33);
        pickup_y = random(0, 17);

        matrix.drawPixel(pickup_x, pickup_y, matrix.Color333(BLUE));
        pickup_exists = true;
    }
}

void setup()
{
    matrix.begin();
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);
    pickup_exists = false;
}

void loop()
{
    get_input();
    pickup();
    delay(50);
}
