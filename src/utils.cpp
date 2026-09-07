#include "utils.h"

const Color Color::White = {255, 255, 255};
const Color Color::Black = {0, 0, 0};
const Color Color::Red = {255, 0, 0};
const Color Color::Green = {0, 255, 0};
const Color Color::Blue = {0, 0, 255};
const Color Color::Yellow = {255, 255, 0};
const Color Color::Cyan = {0, 255, 255};
const Color Color::Magenta = {255, 0, 255};
const Color Color::Orange = {255, 128, 0};
const Color Color::Purple = {128, 0, 255};
const Color Color::Teal = {0, 255, 128};
const Color Color::Lime = {128, 255, 0};
const Color Color::Pink = {255, 0, 128};
const Color Color::LightBlue = {0, 128, 255};
const Color Color::DarkGray = {64, 64, 64};
const Color Color::LightGray = {192, 192, 192};

Color lerp(Color from, Color to, float t)
{
    return Color {
        static_cast<uint8_t>(from.r + (to.r - from.r) * t),
        static_cast<uint8_t>(from.g + (to.g - from.g) * t),
        static_cast<uint8_t>(from.b + (to.b - from.b) * t)
    };
}

Color invertColor(Color color)
{
    return Color {
        255 - color.r,
        255 - color.g,
        255 - color.b
    };
}


uint16_t colorTo565(Color color)
{
    return  ((color.r & 0xF8) << 8) |
            ((color.g & 0xFC) << 3) |
            (color.b >> 3);
}