
const int joy_x_pin = A0;
const int joy_y_pin = A1;

void setup()
{
    pinMode(joy_x_pin, INPUT);
    pinMode(joy_y_pin, INPUT);

    Serial.begin(9600);
    Serial.println("Ready!");
}

void loop()
{
    int x = digitalRead(joy_x_pin);
    int y = digitalRead(joy_y_pin);

    Serial.print("X = ");
    Serial.println(x, DEC);
    Serial.print("Y = ");
    Serial.println(y, DEC);

    delay(500);
}
