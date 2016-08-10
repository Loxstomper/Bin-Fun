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
// 0 index
int width = 31;
int height = 15;
int speed = 25;
#define BLUE 0, 0, 7
#define RED 7, 0, 0
#define GREEN 0, 7, 0
#define WHITE 7, 7, 7
#define BLACK 0, 0, 0

// makes every pixel black
void clear_matrix()
{
    for(int i = 0; i < width; i ++)
        for(int j = 0; j < height; j ++)
            matrix.drawPixel(i, j, matrix.Color333(BLACK));
}

// cycles every single led through r,g,b then turns off
void pixel_test()
{
    for(int x = 0; x <= width; x ++)
        for(int y = 0; y <= height; y ++)
        {
            matrix.drawPixel(x, y, matrix.Color333(RED));
            delay(speed);
            matrix.drawPixel(x, y, matrix.Color333(GREEN));
            delay(speed);
            matrix.drawPixel(x, y, matrix.Color333(BLUE));
            delay(speed);
            matrix.drawPixel(x, y, matrix.Color333(BLACK));
        }
}

// displays the word 'test'
void text_test()
{
    matrix.setCursor(1, 0);
    matrix.setTextSize(1);

    matrix.setTextColor(matrix.Color333(7, 0, 0));
    matrix.print('123');

    delay(1000);
    clear_matrix();
    
    matrix.setTextColor(matrix.Color333(BLUE));
    matrix.setCursor(1, 0);
    matrix.setTextSize(1);
    matrix.print('TESTING');

    delay(500);
    clear_matrix();
}

void setup()
{
  matrix.begin();
  text_test();
}

void loop()
{
    text_test();
}
