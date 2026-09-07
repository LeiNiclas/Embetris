#pragma once

#include <stdint.h>


struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


Color lerp(Color from, Color to, float t);

uint16_t colorTo565(Color col);