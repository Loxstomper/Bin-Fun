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
// flipping axis
const int matrix_width = 16;
const int matrix_height = 32;

// COLOURS
#define BLACK 0, 0, 0
#define RED 7, 0, 0
#define GREEN 0, 7, 0
#define BLUE 0, 0, 7

// Joystick
const int joy_x_pin = A4; // change pin
const int joy_y_pin = A5; // change pin
int joy_x;
int joy_y;

// position - starts off in bottom left when orintated portrait
int x = 0;
int y = 16;
char direction = 'u';

void get_input()
{
    // values are 0 - 1023 on each axis
    joy_x = analogRead(joy_x_pin);
    joy_y = analogRead(joy_y_pin);

    if(joy_x <= 100) // LEFT
        direction = 'd';
    if(joy_x >= 973) // RIGHT
        direction = 'u';
    if(joy_y <= 100) // DOWN
        direction = 'l';
    if(joy_y >= 973) // UP
        direction = 'r';

    update_display();
}

void update_display()
{
    // disable player led
    matrix.drawPixel(x, y, matrix.Color333(BLACK));

    switch( direction )
    {
        case('u'):
            if(x < matrix_height - 1)
            {
                x ++;
            }
            else
            {
                death();
            }
            break;

        case('d'):
            if(x > 0)
            {
                x --;
            }
            else
            {
                death();
            }
            break;

        case('l'):
          if(y > 0)
          {
              y --;
          }
          else
          {
              death();
          }
          break;

        case('r'):
          if(y < matrix_width - 1)
          {
              y ++;
          }
          else
          {
              death();
          }
          break;
    }

    // change led colour (x, y) - ENABLE
    matrix.drawPixel(x, y, matrix.Color333(GREEN));
}

void death()
{
    // disable player led
    matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));

    // Flash screen signalling death
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    matrix.fillScreen(matrix.Color333(BLACK));
    delay(250);
    matrix.fillScreen(matrix.Color333(RED));
    delay(250);
    matrix.fillScreen(matrix.Color333(BLACK));
    delay(250);

    // reset pos
    x = 0;
    y = 0;
    matrix.drawPixel(x, y, matrix.Color333(GREEN));
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
