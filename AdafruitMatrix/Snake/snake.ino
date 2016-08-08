#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

// Matrix
#define CLK 8
#define LAT A3
#define OE 9
#define A A0
#define B A1
#define C A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
const int matrix_width = 32;
const int matrix_height = 16;

// Joystick
const int joy_x_pin = A0; // change pin
const int joy_y_pin = A1; // change pin
int joy_x;
int joy_y;

struct POS
{
    int x;
    int y;
    boolean enabled = false;
}

void get_input()
{
    // values are 0 - 1023 on each axis
    joy_x = digitalRead(joy_x_pin);
    joy_y = digitalRead(joy_y_pin);

    if(joy_x == 0) // LEFT
        direction = 'l';
    if(joy_x == 1023) // RIGHT
        direction = 'r';
    if(joy_y == 0) // DOWN
        direction = 'd';
    if(joy_y == 1023) // UP
        direction = 'u';

    update_display();
}

void update_display()
{
    // disable current leds
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(0, 0, 0));
    // top left is 0,0
    switch( direction )
    {
        case('u')
            if(y > 0)
                y --;
            else
                death();
            break;

        case('d')
            if(y < matrix_height  - 1)
                y ++;
            else
                death();
            break;

        case('l')
        if(x > 0)
            x --;
        else
            death();
        break;

        case('r')
        if(x < matrix_width - 1)
            x ++;
        else
            death();
        break;
    }

    // change led colour (x, y)
    matrix.drawPixel(x, y, matrix.Color333(7, 0, 0));
}

void death()
{
    // disable led
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(0, 0, 0));
    // reset pos
    body[0].x = 0;
    body[1].y = 0;
    // for loop to disable all other parts of the body
    matrix.drawPixel(body[0].x, body[0].y, matrix.Color333(7, 0, 0));
    direction = 'u';
    update_display();
}

void setup()
{
    matrix.begin();
    char direction = 'd';
    POS body [20];
    body[0].x = 0;
    body[0].y = .0;
    body[0].enabled = true;
}

void loop()
{
    get_input();
    delay(100);
}
