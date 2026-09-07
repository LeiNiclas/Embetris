#include "utils.h"


Color lerp(Color from, Color to, float t)
{
    return Color {
        static_cast<uint8_t>(from.r + (to.r - from.r) * t),
        static_cast<uint8_t>(from.g + (to.g - from.g) * t),
        static_cast<uint8_t>(from.b + (to.b - from.b) * t)
    };
}

uint16_t colorTo565(Color color)
{
    return  ((color.r & 0xF8) << 8) |
            ((color.g & 0xFC) << 3) |
            (color.b >> 3);
}