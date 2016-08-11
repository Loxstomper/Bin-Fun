#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>


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

    switch( direction )
    {
        case('u'):
          Serial.println("UP!");
          break;

        case('d'):
          Serial.println("DOWN!");
          break;

        case('l'):
          Serial.println("LEFT!");
          break;

        case('r'):
          Serial.println("RIGHT!");
          break;
    }
}

void setup()
{
    Serial.begin(9600);
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);
}

void loop()
{
    get_input();
    delay(1000);
}
