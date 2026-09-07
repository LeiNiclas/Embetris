#include "Display.h"

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "utils.h"

namespace {
    constexpr int LCD_CS_PIN = 14;
    constexpr int LCD_RST_PIN = 13;
    constexpr int LCD_DC_PIN = 12;
    constexpr int LCD_SPI_MOSI_PIN = 11;
    constexpr int LCD_SPI_SCK_PIN = 10;
    constexpr int LCD_SPI_MISO_PIN = 9;
    constexpr int LCD_LED_PIN = 46;
    constexpr int LCD_LED_CHANNEL = 5;
    constexpr int LCD_LED_FREQUENCY = 5000;
    constexpr int LCD_LED_RESOLUTION = 8;
    constexpr int LCD_WIDTH = 320;
    constexpr int LCD_HEIGHT = 480;

    TFT_eSPI tft(LCD_WIDTH, LCD_HEIGHT);
}


/// @brief Initializes the LCD.
void Display::init()
{
    config = DisplayConfig();
    config.brightness = 255;
    config.rotation = 0;
    config.backgroundColor = TFT_BLACK;
    config.gridColor = TFT_DARKGREY;

    pinMode(LCD_CS_PIN, OUTPUT);
    digitalWrite(LCD_CS_PIN, LOW);

    tft.init();
    tft.setRotation(0);

    pinMode(LCD_LED_PIN, OUTPUT);
    digitalWrite(LCD_LED_PIN, HIGH);

    ledcSetup(LCD_LED_CHANNEL, LCD_LED_FREQUENCY, LCD_LED_RESOLUTION);
    ledcAttachPin(LCD_LED_PIN, LCD_LED_CHANNEL);
    setBrightness(config.brightness);
}

/// @brief Updates the brigthness of the LCD screen.
void Display::setBrightness(uint8_t brightness)
{
    config.brightness = brightness;
    ledcWrite(LCD_LED_CHANNEL, brightness);
}

/// @brief Sets the rotation / orientation of the screen.
/// Can only have values [0..3]
void Display::setRotation(uint8_t rotation)
{
    assert(rotation < 4);

    config.rotation = rotation;
    tft.setRotation(rotation);
}

/// @brief Updates the default background color of all screens. 
void Display::setBackgroundColor(Color color)
{
    config.backgroundColor = colorTo565(color);
}

/// @brief Updates the grid color of the game screen.
void Display::setGridColor(Color color)
{
    config.gridColor = colorTo565(color);
}


/// @brief Draws a single pixel at (x, y). 
void Display::drawPixel(int x, int y, Color color)
{
    tft.drawPixel(x, y, colorTo565(color));
}

/// @brief Draws a line from (x1, y1) to (x2, y2).
void Display::drawLine(int x1, int y1, int x2, int y2, Color color)
{
    tft.drawLine(x1, y1, x2, y2, colorTo565(color));
}

/// @brief Draws a multicolored line from (x1, y1) to (x2, y2)
/// using a specific amount of segments. More segments = higher color resolution.
void Display::drawLine(int x1, int y1, int x2, int y2, Color col1, Color col2, uint8_t segments)
{
    float dx = (x1 - x2) / segments;
    float dy = (y1 - y2) / segments;

    Color color;

    x2 = x1;
    y2 = y1;

    for (size_t i = 0; i < segments; ++i)
    {
        x1 = x2;
        y1 = y2;

        x2 += dx;
        y2 += dy;

        color = lerp(col1, col2, float(i) / float(segments - 1));
        drawLine(x1, y1, x2, y2, color);
    }
}

/// @brief Draws a rect starting from (x, y). Outline only.
void Display::drawRect(int x, int y, int width, int height, Color color)
{
    tft.drawRect(x, y, width, height, colorTo565(color));
}

/// @brief Draws a filled rect starting from (x, y).
void Display::fillRect(int x, int y, int width, int height, Color color)
{
    tft.fillRect(x, y, width, height, colorTo565(color));
}

/// @brief Draws a circle centered at (x, y). Outline only.
void Display::drawCircle(int x, int y, int radius, Color color)
{
    tft.drawCircle(x, y, radius, colorTo565(color));
}

/// @brief Draws a filled circle centered at (x, y).
void Display::fillCircle(int x, int y, int radius, Color color)
{
    tft.fillCircle(x, y, radius, colorTo565(color));
}

/// @brief Draws text starting from (x, y).
void Display::drawText(int x, int y, uint8_t size, Color color, Color bgColor, const char* text)
{
    tft.setTextColor(colorTo565(color), colorTo565(bgColor), false);
    tft.setTextSize(size);
    tft.drawString(text, x, y);
}

/// @brief Draws text starting from (x, y).
void Display::drawText(Color color, Color bgColor, const char* text)
{
    tft.setTextColor(colorTo565(color), colorTo565(bgColor), false);
    tft.drawString(text, tft.getCursorX(), tft.getCursorY());
}


/// @brief Clears the screen with the current background color set in the current config.
void Display::clear()
{
    tft.fillScreen(config.backgroundColor);
}

/// @brief Obsolete at the moment. Might become necessary for sprites later on.
void Display::present() {}