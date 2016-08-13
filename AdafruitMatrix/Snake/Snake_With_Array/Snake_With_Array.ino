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
// 0 index
const int matrix_width = 15; // inverted axis due to using portrait orientation
const int matrix_height = 31; // inverted axis due to using portrait orientation

// ----- Colours ----- //
#define BLACK 0, 0, 0
#define RED 7, 0, 0
#define GREEN 0, 7, 0
#define BLUE 0, 0, 7
#define WHITE 1, 1, 1
// maybe make a library of colours so to access it would be COLOUR.blue, etc.

// ########## Joystick ##########//
const int joy_x_pin = A4;
const int joy_y_pin = A5;
int joy_x; // 0-1023
int joy_y; // 0-1023

// ########## Player ########## //
char direction;
int score;
int max_length = 20;
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

SNAKE body [10];
// ########## Functions ########## //
void get_input()
{
    //Serial.println("Getting Input");
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

void pickup()
{
    // spawn pickup if one does not exist
    if(pickup_exists == false)
    {
        pickup_x = random(0, 32);
        pickup_y = random(0, 16);

        matrix.drawPixel(pickup_x, pickup_y, matrix.Color333(BLUE));
        pickup_exists = true;
    }
}

void death()
{
    //Serial.println("Death");
    // pretty animation - makes the entire body red from head to tail
    for(int i = 0; i < length; i ++)
    {
        matrix.drawPixel(body[i].x, body[i].y, matrix.Color333(RED));
        delay(100);
    }
    delay(500);
    // make entire screen black cant be bothered doing another for loop
    clear_display();

    // disable all elements except the head - can merge with above for loop
    for(int i = 1; i < length - 1; i ++)
        body[i].enabled = false;

    // Flash screen signalling death- change to a sad face or something better
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    clear_display();
    delay(250);
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    clear_display();
    delay(250);

    // reset player
    score = 0;
    length = 1;
    body[0].x = 0;
    body[0].y = 15;
    matrix.drawPixel(body[0].x,  body[0].y, matrix.Color333(GREEN));
    direction = 'u';

    update_display();
    delay(1000); // wait 1 second to give time for the player
    pickup_exists = false;
}

void clear_display()
{
  matrix.fillScreen(matrix.Color333(BLACK));
}

void update_display()
{
    // disable player led
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(BLACK));

    // translate direction into movement, only moving the body[0] which is the head
    switch(direction)
    {
        case('u'):
            //Serial.println("Up");
            body[0].x ++;
            if(body[0].x > matrix_height) // if hit the top go to botton
                body[0].x = 0;
            break;

        case('d'):
            //Serial.println("Down");
            body[0].x --;
            if(body[0].x < 0) // if hit the bottom go to top
                body[0].x = 32;
            break;

        case('l'):
            //Serial.println("Left");
            body[0].y --;
            if(body[0].y < 0) // if hit the left go to right
                body[0].y = 16;
          break;

        case('r'):
            //Serial.println("Right");
            body[0].y ++;
            if(body[0].y > matrix_width) // if hit the right go to right
                body[0].y = 0;
          break;
    }
    // turn on LED in the new position of the head
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(GREEN));

    // is on a pickup
    if(body[0].x == pickup_x && body[0].y == pickup_y && pickup_exists == true) // needs pickup_exists otherwise will change colour on the coord after picked up
    {
        //Serial.println("On a pickup");
        length ++;
        body[length - 1].enabled = true; // 0 index
        // "remove" pickup - stops from spawning more
        // PICKUP ONLY WORKS ONCE
        pickup_exists == false;
        score += 10; // add 10 to the score for picking up
    }

    // update entire body - maybe issue with element 1 and element 0, if there is, make element 1 = element 0 before element 0 is modified and remeove element 1 from for loop
    // having an invisible snake the entire time and its components are just enabled
    for(int i = 1; i < max_length - 1; i ++) // make a variable for max length, DOUBLE CHECK THIS ON THE LIMIT
    {

//###############################################################################################//
        // ## UPDATES POSITION ## //
        // if these are enable the matrix stuffs up
        //body[i].x = body[i-1].x;
        //body[i].y = body[i-1].y;

        // ## DRAWS ENABLED BODY ELEMENTS ## //

        // not using a for loop
        /*if(i < length - 1) // enable all that should be enabled
          body[i].enabled = true;

        if(body[i].enabled == true) // draw
          matrix.drawPixel(body[i].x, body[i].y, matrix.Color333(GREEN));*/

        
        /*for(int j = 1; j < length - 1; j ++)
        {
            if(body[j].enabled == false) // saves enabling elements that are allready enabled
                body[j].enabled = true;

            matrix.drawPixel(body[j].x, body[j].y, matrix.Color333(GREEN));
        }*/
    }
//###############################################################################################
    // collision - if snakes head touches its body
    /*for(int i = 1; i < length + 1; i ++)
    {
        Serial.println("Collision detection");
        if(body[0].x == body[i].x && body[0].y == body[i].y && body[i].enabled == true)
            //death();
            Serial.println("Should be dead");
    }*/
}

void game_start()
{
  matrix.setCursor(1, 0);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(1, 1, 1));
  matrix.print("Start");
  matrix.setCursor(1, 8);
  matrix.print("Game!");
  delay(2500);
  clear_display();
}

void setup()
{
    //Serial.begin(9600);
    matrix.begin();
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);
    pickup_exists = false;
    delay(200);
    clear_display();

    // initilise snake
    length = 1;
    body[0].x = 0;
    body[0].y = 15;
    body[0].enabled = true;
    direction = 'u';

    // set values shouldnt need to do this
    /*for(int i = 1; i < max_length - 1; i ++)
    {
      body[i].x = 0;
      body[i].y = 0;
      body[i].enabled = false;
    }*/

    game_start();
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(GREEN));
    //Serial.println("Snake Initilized");
}

void loop()
{
    get_input();
    pickup();
    delay(200);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
