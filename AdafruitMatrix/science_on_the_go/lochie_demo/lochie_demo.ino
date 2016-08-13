#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

#define CLK 8
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// ##### COLOURS ##### //
#define RED 1, 0, 0
#define GREEN 0, 1, 0
#define BLUE 0, 0, 1
#define BLACK 0, 0, 0
#define WHITE 1, 1, 1

void clear_screen()
{
    matrix.fillScreen(matrix.Color333(BLACK));
}

void police_lights()
{
    for(int i = 0; i < 20; i ++)
    {
        // RED
        matrix.fillRect(0, 0, 16, 16, matrix.Color333(RED));
        delay(50);
        matrix.fillRect(0, 0, 16, 16, matrix.Color333(BLACK));
        delay(50);
        matrix.fillRect(0, 0, 16, 16, matrix.Color333(RED));
        delay(50);
        matrix.fillRect(0, 0, 16, 16, matrix.Color333(BLACK));

        // BLUE
        matrix.fillRect(16, 0, 32, 32, matrix.Color333(BLUE));
        delay(50);
        matrix.fillRect(16, 0, 32, 32, matrix.Color333(BLACK));
        delay(50);
        matrix.fillRect(16, 0, 32, 32, matrix.Color333(BLUE));
        delay(50);
        matrix.fillRect(16, 0, 32, 32, matrix.Color333(BLACK));
    }
}

void rgb()
{
    matrix.fillScreen(matrix.Color333(RED)); // red
    delay(500);
    clear_screen();
    delay(500);
    matrix.fillScreen(matrix.Color333(GREEN)); // green
    delay(500);
    clear_screen();
    delay(500);
    matrix.fillScreen(matrix.Color333(BLUE)); // blue
    delay(500);
}

void random_colours()
{
  matrix.fillScreen(matrix.Color333(WHITE));
    for(int x = 0; x <= 31; x ++)
        for(int y = 0; y <= 15; y ++)
        {
            matrix.drawPixel(x, y, matrix.Color333(random(0, 8), random(0, 8), random(0, 8)));
            delay(50);
        }
        matrix.fillScreen(matrix.Color333(BLACK));
}

void setup()
{
    matrix.begin();
}

void loop()
{
    matrix.setCursor(1, 8);
    matrix.setTextSize(1);
    matrix.print("DEMO");
    delay(2000);
    police_lights();
    rgb();
    random_colours();
}
