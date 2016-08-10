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
const int joy_x_pin = A4; // change pin
const int joy_y_pin = A5; // change pin
int joy_x;
int joy_y;

// position - starts off in top left
int x = 0;
int y = 0;
char direction = 'd';

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
    matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
    // top left is 0,0
    switch( direction )
    {
        case('u'):
            if(y > 0)
            {
                y --;
            }
            else
            {
                death();
            }
            break;

        case('d'):
            if(y < matrix_height  - 1)
            {
                y ++;
            }
            else
            {
                death();
            }
            break;

        case('l'):
          if(x > 0)
          {
              x --;
          }
          else
          {
              death();
          }
          break;

        case('r'):
          if(x < matrix_width - 1)
          {
              x ++;
          }
          else
          {
              death();
          }
          break;
    }

    // change led colour (x, y)
    matrix.drawPixel(x, y, matrix.Color333(7, 0, 0));
}

void death()
{
    // disable led
    matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
    // reset pos
    x = 0;
    y = 1;
    // light up (0,1)
    matrix.drawPixel(x, y, matrix.Color333(7, 0, 0));
    direction = 'u';
    update_display();
}

void setup()
{
    matrix.begin();
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);
}

void loop()
{
    get_input();
    delay(100);
}
