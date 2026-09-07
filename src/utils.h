#pragma once

#include <stdint.h>


struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Yellow;
    static const Color Orange;
    static const Color Purple;
    static const Color Teal;
    static const Color Lime;
    static const Color Pink;
    static const Color LightBlue;
    static const Color DarkGray;
    static const Color LightGray;
};

Color lerp(Color from, Color to, float t);

Color invertColor(Color color);

uint16_t colorTo565(Color col);