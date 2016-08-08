#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

#define CLK 8
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup()
{
    matrix.begin();
}

void loop()
{
    matrix.fillScreen(matrix.Color333(7, 0, 0)); // red
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // black
    delay(1000);
    matrix.fillScreen(matrix.Color333(0, 7, 0)); // green
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // black
    delay(1000);
    matrix.fillScreen(matrix.Color333(0, 0, 7)); // blue
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // black
    delay(1000);
}
