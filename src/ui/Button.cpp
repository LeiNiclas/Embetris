#include "Button.h"
#include <hardware/Display.h>


bool Button::isInside(int x, int y)
{
    return  (Button::x >= x) &&
            (Button::x <= x + width) &&
            (Button::y >= y) &&
            (Button::y <= y + height); 
}


Button::Button(
    Display &display,
    int x, int y,
    int width, int height,
    Color color, Color borderColor,
    int textSize, char* buttonText, Color textColor
)
{
    Button::x = x;
    Button::y = y;
    Button::width = width;
    Button::height = height;
    Button::color = color;
    Button::borderColor = borderColor;
    Button::textSize = textSize;
    Button::buttonText = buttonText;
    Button::textColor = textColor;

    display.drawRect(x, y, width, height, borderColor);
    display.fillRect(x+1, y+1, width-2, height-2, color);

    display.drawText(x, y + (height/2), textSize, textColor, color, buttonText);

    currentlyPressed = false;
}


bool Button::isHovered(int touchX, int touchY, bool touched)
{
    bool inside = isInside(touchX, touchY);
    
    currentlyPressed = touched && inside;

    return inside;
}


bool Button::isPressedThisFrame(int touchX, int touchY, bool touched)
{
    bool inside = isInside(touchX, touchY);

    if (inside && currentlyPressed && !touched)
    {
        currentlyPressed = false;
        return true;
    }

    currentlyPressed = touched && inside;
    return false;
}


void Button::highlight(Display &display)
{
    display.drawRect(x, y, width, height, invertColor(borderColor));
    display.fillRect(x+1, y+1, width-2, height-2, invertColor(color));

    display.drawText(x, y + (height/2), textSize, textColor, invertColor(color), buttonText);
}


void Button::unHighlight(Display &display)
{
    display.drawRect(x, y, width, height, borderColor);
    display.fillRect(x+1, y+1, width-2, height-2, color);

    display.drawText(x, y + (height/2), textSize, textColor, color, buttonText);
}