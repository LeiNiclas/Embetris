#include "Touchscreen.h"

#include <Arduino.h>
#include <Wire.h>

namespace {
    constexpr int TOUCH_IRQ_PIN = 17;
    constexpr int TOUCH_I2C_SDA_PIN = 18;
    constexpr int TOUCH_I2C_SCL_PIN = 3;
    constexpr int TOUCH_I2C_RST_PIN = 8;
    constexpr uint8_t TOUCH_I2C_ADDRESS = 0x38;
    constexpr uint8_t FT6336_REG_TD_STATUS = 0x02;
    constexpr uint8_t FT6336_REG_P1_XH = 0x03;
}


void Touchscreen::init()
{
    pinMode(TOUCH_IRQ_PIN, INPUT);
    pinMode(TOUCH_I2C_RST_PIN, OUTPUT);

    digitalWrite(TOUCH_I2C_RST_PIN, LOW);
    delay(5);
    digitalWrite(TOUCH_I2C_RST_PIN, HIGH);
    delay(50);

    Wire.begin(TOUCH_I2C_SDA_PIN, TOUCH_I2C_SCL_PIN);
}


void Touchscreen::readValues(uint16_t &x, uint16_t &y, bool &touched)
{
    x = 0;
    y = 0;
    touched = false;

    Wire.beginTransmission(TOUCH_I2C_ADDRESS);
    Wire.write(FT6336_REG_TD_STATUS);

    if (Wire.endTransmission(false) != 0 ||
        Wire.requestFrom(TOUCH_I2C_ADDRESS, static_cast<uint8_t>(1)) != 1)
    {
        return;
    }

    uint8_t touchCount = Wire.read() & 0x0F;
    if (touchCount == 0 || touchCount > 2)
    {
        return;
    }

    Wire.beginTransmission(TOUCH_I2C_ADDRESS);
    Wire.write(FT6336_REG_P1_XH);

    if (Wire.endTransmission(false) != 0 ||
        Wire.requestFrom(TOUCH_I2C_ADDRESS, static_cast<uint8_t>(4)) != 4)
    {
        return;
    }

    uint8_t xHigh = Wire.read();
    uint8_t xLow = Wire.read();
    uint8_t yHigh = Wire.read();
    uint8_t yLow = Wire.read();

    x = ((xHigh & 0x0F) << 8) | xLow;
    y = ((yHigh & 0x0F) << 8) | yLow;
    touched = true;
}