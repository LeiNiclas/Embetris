#include <Arduino.h>

#include <hardware/Display.h>
#include <hardware/Joystick.h>
#include <hardware/Touchscreen.h>
#include "utils.h"
#include "menu/MainMenu.h"

#define WAIT_TIME 1000 

Display display;
Joystick joystick;
Touchscreen touchscreen;

Color RED = { 255, 0, 0 };
Color GREEN = { 0, 255, 0 };
Color BLUE = { 0, 0, 255 };

Color YELLOW = { 255, 255, 0 };
Color MAGENTA = { 255, 0, 255 };
Color CYAN = { 0, 255, 255 };

Color BLACK = { 0, 0, 0 };
Color WHITE = { 255, 255, 255 };
Color DARK_RED = { 96, 0, 0 };

namespace {
    constexpr int LABEL_X = 20;
    constexpr int SQUARE_X = 150;
    constexpr int FIRST_ROW_Y = 30;
    constexpr int ROW_SPACING = 45;
    constexpr int SQUARE_SIZE = 30;
    constexpr int JOYSTICK_CIRCLE_X = 160;
    constexpr int JOYSTICK_CIRCLE_Y = 390;
    constexpr int JOYSTICK_CIRCLE_RADIUS = 70;
    constexpr int JOYSTICK_POSITION_RADIUS = 12;
}


void displayTest()
{
    display.setBrightness(255);
    display.clear();
    display.drawPixel(1, 1, RED);
    delay(WAIT_TIME);
    display.drawPixel(100, 100, GREEN);
    delay(WAIT_TIME);

    display.drawLine(200, 300, 300, 200, BLUE);
    delay(WAIT_TIME);
    display.drawLine(150, 250, 250, 150, MAGENTA, YELLOW, 50);
    delay(WAIT_TIME);

    display.setBrightness(128);
    delay(WAIT_TIME);
    display.setBrightness(64);
    delay(WAIT_TIME);
    display.setBrightness(0);
    delay(WAIT_TIME);
}


void joystickTest()
{
    static bool screenInitialized = false;

    if (!screenInitialized)
    {
        display.clear();
        display.drawText(LABEL_X, FIRST_ROW_Y, 2, WHITE, BLACK, "Left");
        display.drawText(LABEL_X, FIRST_ROW_Y + ROW_SPACING, 2, WHITE, BLACK, "Right");
        display.drawText(LABEL_X, FIRST_ROW_Y + 2 * ROW_SPACING, 2, WHITE, BLACK, "Up");
        display.drawText(LABEL_X, FIRST_ROW_Y + 3 * ROW_SPACING, 2, WHITE, BLACK, "Down");
        screenInitialized = true;
    }

    float x;
    float y;
    bool pressed;
    joystick.readValues(x, y, pressed);

    float averageX;
    float averageY;
    
    joystick.readAverageValues(averageX, averageY);

    display.fillRect(SQUARE_X, FIRST_ROW_Y, SQUARE_SIZE, SQUARE_SIZE,
                     joystick.isLeft() ? GREEN : DARK_RED);
    display.fillRect(SQUARE_X, FIRST_ROW_Y + ROW_SPACING, SQUARE_SIZE, SQUARE_SIZE,
                     joystick.isRight() ? GREEN : DARK_RED);
    display.fillRect(SQUARE_X, FIRST_ROW_Y + 2 * ROW_SPACING, SQUARE_SIZE, SQUARE_SIZE,
                     joystick.isUp() ? GREEN : DARK_RED);

    display.drawCircle(JOYSTICK_CIRCLE_X, JOYSTICK_CIRCLE_Y,
                       JOYSTICK_CIRCLE_RADIUS, WHITE);

    int positionX = JOYSTICK_CIRCLE_X +
                    averageX * (JOYSTICK_CIRCLE_RADIUS - JOYSTICK_POSITION_RADIUS);
    int positionY = JOYSTICK_CIRCLE_Y -
                    averageY * (JOYSTICK_CIRCLE_RADIUS - JOYSTICK_POSITION_RADIUS);

    static int previousPositionX = JOYSTICK_CIRCLE_X;
    static int previousPositionY = JOYSTICK_CIRCLE_Y;
    
    display.fillCircle(previousPositionX, previousPositionY,
                       JOYSTICK_POSITION_RADIUS, BLACK);
    display.fillCircle(positionX, positionY, JOYSTICK_POSITION_RADIUS, GREEN);
    
    previousPositionX = positionX;
    previousPositionY = positionY;

    display.fillRect(SQUARE_X, FIRST_ROW_Y + 3 * ROW_SPACING, SQUARE_SIZE, SQUARE_SIZE,
                     joystick.isDown() ? GREEN : DARK_RED);
    
    delay(16.67);
}


void touchscreenTest()
{
    static bool screenInitialized = false;

    if (!screenInitialized)
    {
        display.clear();
        display.drawText(20, 30, 2, WHITE, BLACK, "X:");
        display.drawText(20, 60, 2, WHITE, BLACK, "Y:");
        display.drawText(20, 90, 2, WHITE, BLACK, "Touched:");
        screenInitialized = true;
    }

    uint16_t x;
    uint16_t y;
    bool touched;
    touchscreen.readValues(x, y, touched);

    char value[16];

    snprintf(value, sizeof(value), "%u", x);
    display.drawText(140, 30, 2, WHITE, BLACK, value);

    snprintf(value, sizeof(value), "%u", y);
    display.drawText(140, 60, 2, WHITE, BLACK, value);

    display.drawText(140, 90, 2, WHITE, BLACK, touched ? "Yes" : "No");

    delay(50);
}


MainMenu mainMenu;


void setup()
{
    Serial.begin(115200);
    while(!Serial);
    display.init();
    joystick.init();
    touchscreen.init();
    mainMenu.initMenu();
}


void loop()
{
    static bool firstRun = true;
    // displayTest();
    // joystickTest();
    if (firstRun)
    {
        if (mainMenu.show(display, joystick, touchscreen, millis()))
        {
            firstRun = false;
        }
    } else 
    {
        touchscreenTest();
    }
}