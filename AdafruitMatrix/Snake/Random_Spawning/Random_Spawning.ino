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
#define BLUE 0, 0, 7
#define BLACK 0, 0, 0

// ########## Pickups i.e. fruit ########## //
boolean pickup_exists;
int pickup_x;
int pickup_y;

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
    pickup_exists = false;
}

void loop()
{
    pickup();
    delay(1000);
    matrix.drawPixel(pickup_x, pickup_y, matrix.Color333(BLACK));
    pickup_exists = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
