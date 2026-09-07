#pragma once

#include <stdint.h>
#include "utils.h"

struct DisplayConfig
{
    uint8_t brightness;
    uint8_t rotation;
    uint16_t backgroundColor;
    uint16_t gridColor;
};

class Display {
    public:
        void init();
        void setBrightness(uint8_t brightness);
        void setRotation(uint8_t rotation);
        void setBackgroundColor(Color color);
        void setGridColor(Color color);

        void drawPixel(int x, int y, Color color);
        void drawLine(int x1, int y1, int x2, int y2, Color color);
        void drawLine(int x1, int y1, int x2, int y2, Color col1, Color col2, uint8_t segments);
        void drawRect(int x, int y, int width, int height, Color color);
        void fillRect(int x, int y, int width, int height, Color color);
        void drawCircle(int x, int y, int radius, Color color);
        void fillCircle(int x, int y, int radius, Color color);
        void drawText(int x, int y, uint8_t size, Color color, Color bgColor, const char* text);

        void clear();

        void present();
    
    
    private:
        DisplayConfig config;
};