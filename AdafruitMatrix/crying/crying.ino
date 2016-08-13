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


// ########## Player ########## //
//int x = 0;
//int y = 16;
char direction;
int score;
int length;

// ########## Structures ########## //
typedef struct
{
    int x;
    int y;
    boolean enabled = false;
} SNAKE;

SNAKE body [10];

void setup()
{
    matrix.begin();
    matrix.fillScreen(matrix.Color333(BLUE));
    delay(200);
    matrix.fillScreen(matrix.Color333(BLACK));

    // initilise snake
    body[0].x = 0;
    body[0].y = 15;
    body[0].enabled = true;
    body[1].x = 2;
    body[1].y = 15;
    body[1].enabled = true;
    body[2].x = 3;
    body[2].y = 10;
    body[2].enabled = true;
    body[3].x = 8;
    body[3].y = 2;
    body[3].enabled = true;
    body[4].x = 4;
    body[4].y = 3;
    body[4].enabled = true;
}

void loop()
{
    for(int i = 0; i < 3; i ++)
    {
        matrix.drawPixel(body[i].x, body[i].y, matrix.Color333(GREEN));
        delay(100);
    }
    delay(200);
    matrix.fillScreen(matrix.Color333(BLACK));
    delay(200);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
