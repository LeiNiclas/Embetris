#include "Joystick.h"

#include <Arduino.h>

/// Note: The button of the joystick is currently not being used.

namespace {
    constexpr int JOYSTICK_X_PIN = 2;
    constexpr int JOYSTICK_Y_PIN = 1;
    // constexpr int JOYSTICK_BUTTON_PIN = 3;
    constexpr int JOYSTICK_MAX_VALUE = 4096;
    constexpr int JOYSTICK_MIN_VALUE = 0;

    constexpr float LEFT_THRESHOLD = -0.5f;
    constexpr float RIGHT_THRESHOLD = 0.5f;
    constexpr float UP_THRESHOLD = 0.5f;
    constexpr float DOWN_THRESHOLD = -0.5f;
}

/// @brief Initializes the Joystick
void Joystick::init()
{
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
    // pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

    runningIndex = 0;
    
    for (size_t i = 0; i < 10; ++i)
    {
        recentValuesX[i] = 0;
        recentValuesY[i] = 0;
    }
}

/// @brief Reads the joystick values into the given parameters.
/// x and y are normalized, i.e. in [-1, 1)
void Joystick::readValues(float &x, float &y, bool &buttonPressed)
{
    int analogX = analogRead(JOYSTICK_X_PIN);
    int analogY = analogRead(JOYSTICK_Y_PIN);
    
    // Normalization to [-1, 1)
    int mid = (JOYSTICK_MAX_VALUE + 1) / 2;
    
    x = (float(analogX) / float(mid)) - 1.0f;
    y = (float(analogY) / float(mid)) - 1.0f;

    // buttonPressed = (digitalRead(JOYSTICK_BUTTON_PIN) == LOW);

    recentValuesX[runningIndex] = x;
    recentValuesY[runningIndex] = y;
    runningIndex = (runningIndex + 1) % 10;
}

/// @brief Reads the average joystick values into x and y using the last recorded values.
void Joystick::readAverageValues(float &x, float &y)
{
    x = 0;
    y = 0;

    for (size_t i = 0; i < 10; ++i)
    {
        x += recentValuesX[i];
        y += recentValuesY[i];
    }

    x /= 10;
    y /= 10;
}

/// @brief Checks if the Joystick is held to the left.
bool Joystick::isLeft()
{
    float sum = 0;

    for (size_t i = 0; i < 10; ++i)
        sum += recentValuesX[i];

    return sum / 10 <= LEFT_THRESHOLD;
}

/// @brief Checks if the Joystick is held to the right.
bool Joystick::isRight()
{
    float sum = 0;

    for (size_t i = 0; i < 10; ++i)
        sum += recentValuesX[i];
    
    return sum / 10 >= RIGHT_THRESHOLD;
}

/// @brief Checks if the Joystick is held upwards.
bool Joystick::isUp()
{
    float sum = 0;

    for (size_t i = 0; i < 10; ++i)
        sum += recentValuesY[i];
    
    return sum / 10 >= UP_THRESHOLD;
}

/// @brief Checks if the Joystick is held downwards.
bool Joystick::isDown()
{
    float sum = 0;

    for (size_t i = 0; i < 10; ++i)
        sum += recentValuesY[i];
    
    return sum / 10 <= DOWN_THRESHOLD;
}