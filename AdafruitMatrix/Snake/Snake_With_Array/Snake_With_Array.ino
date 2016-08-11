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
//int x = 0;
//int y = 16;
char direction;
int score;
int length;

// ########## Pickups i.e. fruit ########## //
boolean pickup_exists;
int pickup_x;
int pickup_y;

// ########## Structures ########## //
typedef struct
{
    int x;
    int y;
    boolean enabled = false;
} SNAKE;

// ########## Initializes Snake ########## //
SNAKE body [10];

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
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(BLACK));

    // translate direction into movement, only moving the body[0] which is the head
    switch(direction)
    {
        case('u'):
            body[0].x ++;
            if(body[0].x > matrix_height) // if hit the top go to botton
                body[0].x = 0;
            break;

        case('d'):
            body[0].x --;
            if(body[0].x < 0) // if hit the bottom go to top
                body[0].x = 32;
            break;

        case('l'):
            body[0].y --;
            if(body[0].y < 0) // if hit the left go to right
                body[0].y = 16;
          break;

        case('r'):
            body[0].y ++;
            if(body[0].y > 16) // if hit the right go to right
                body[0].y = 0;
          break;
    }
    // turn on LED in the new position of the head
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(GREEN));

    // is on a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y && pickup_exists == true) // needs pickup_exists otherwise will change colour on the coord after picked up
    {
        length ++;
        body[length - 1].enabled = true; // 0 index
        // "remove" pickup - stops from spawning more
        pickup_exists == false;
        score += 10; // add 10 to the score for picking up
    }

    // update entire body - maybe issue with element 1 and element 0, if there is, make element 1 = element 0 before element 0 is modified and remeove element 1 from for loop
    // having an invisible snake the entire time and its components are just enabled
    for(int i = 1; i < 20; i ++) // make a variable for max length, DOUBLE CHECK THIS ON THE LIMIT
    {
        body[i].x = body[i-1].x;
        body[i].y = body[i-1].y;
        // only lights up the enabled parts
        for(int j = 1; j < length + 1; j ++)
        {
            if(body[j].enabled == false) // saves enabling elements that are allready enabled
                body[j}.enabled = true;

            matrix.drawPixel(body[j].x, body[j].y, matrix.Color333(GREEN));
    }

    // collision - if snakes head touches its body
    for(int i = 1; i < length + 1; i ++)
    {
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
            death();
    }
}

void death()
{
    // pretty animation - makes the entire body red from head to tail
    for(int i = 0; i < length; i ++)
    {
        matrix.drawPixel(body[i].x, body[i].y, matrix.Color333(RED));
        delay(100);
    }
    delay(500);
    // make entire screen black cant be bothered doing another for loop
    matrix.fillScreen(matrix.Color333(BLACK));

    // disable all elements except the head - can merge with above for loop
    for(int i = 1; i < length - 1; i ++)
        body[i].enabled = false;

    // Flash screen signalling death- change to a sad face or something better
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
    length = 1;
    body[0].x = 0;
    body[0].y = 16;
    matrix.drawPixel(body[0].x,  body[0].y, matrix.Color333(GREEN));
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

    // initilise snake
    length = 1;
    body[0].x = 0;
    body[0].y = 16;
    body[0].enabled = true;
    direction = 'u';
}

void loop()
{
    get_input();
    pickup();
    delay(50);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
