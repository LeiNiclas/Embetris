#pragma once

#include "utils.h"
#include <hardware/Display.h>

class Button {
    public:
        Button(
            Display &display,
            int x, int y,
            int width, int height,
            Color color, Color borderColor,
            int textSize, char* buttonText, Color textColor
        );
        
        bool isPressedThisFrame(int touchX, int touchY, bool touched);
        bool isHovered(int touchX, int touchY, bool touched);

    private:
        int x;
        int y;
        int width;
        int height;
        Color color;
        Color borderColor;
        int textSize;
        char* buttonText;
        Color textColor;
        bool currentlyPressed;

        bool isInside(int x, int y);
        
        void highlight(Display &display);
        void unHighlight(Display &display);
};