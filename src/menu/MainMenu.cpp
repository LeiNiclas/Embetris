#include "MainMenu.h"

#define DEBUG       1

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif



void MainMenu::initMenu() 
{ 
    //  Initialize internal variables
    bStartGame = false; 

    //  Clear the screen or perform any other necessary setup for the main menu.
    bMenuUpdateNeeded = true;

    //  Initialize the start button pressed state
    bStartButtonPressed = false;

}

typedef bool TetrisShape_t[5][3];

TetrisShape_t shapeT = {
    { true,  true,  true},
    { false, true, false},
    { false, true, false},
    { false, true, false},
    { false, true, false}
};
TetrisShape_t shapeE = {
    { true,  true,  true},
    { true,  false, false},
    { true,  true, false},
    { true,  false, false},
    { true,  true,  true}
};
TetrisShape_t shapeR = {
    { true,  true,  true},
    { true, false,  true},
    { true, true,  false},
    { true, false,  true},
    { true,  false,  true}
};
TetrisShape_t shapeI = {
    { true,  false, false},
    { false, false, false},
    { true, false, false},
    { true, false, false},
    { true,  false, false}
};
TetrisShape_t shapeS = {
    { true,  true,  true},
    { true,  false,  false},
    { true,  true,  true},
    { false,  false,  true},
    { true,  true,  true}
};

void DrawTetrisBlock(Display &display, int x, int y, Color color)
{
    display.drawRect(x, y, 20, 20, Color::DarkGray); // Draw the Tetris block background
    display.fillRect(x + 1, y + 1, 18, 18, color); // Fill the Tetris block background
}

void DrawTetrisShape(Display &display, int x, int y, Color color, TetrisShape_t shape)
{
    for (int row = 0; row < 5; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (shape[row][col])
            {
                DrawTetrisBlock(display, x + col * 20, y + row * 20, color);
            }
        }
    }
}

bool MainMenu::show(Display &display, Joystick &joystick, Touchscreen &touchscreen, uint32_t timestamp)
{
    static uint32_t lastUpdate = 0;

    //  Check any time relevant function
    if (timestamp - lastUpdate < 1000) // Update every 1 second
    {
//        return false;
    } else 
    {
        lastUpdate = timestamp;
    }

    //  Check if the menu needs to be updated
    if (bMenuUpdateNeeded)
    {
        //  Mark the menu as needing an update.
        display.clear(); // Clear the display when showing the menu for the first time.
        display.drawText(40, 20, 4, Color::Red, Color::Black, "T");
        display.drawText(80, 20, 4, Color::Blue, Color::Black, "E");
        display.drawText(120, 20, 4, Color::Green, Color::Black, "T");
        display.drawText(160, 20, 4, Color::Yellow, Color::Black, "R");
        display.drawText(200, 20, 4, Color::Cyan, Color::Black, "I");
        display.drawText(240, 20, 4, Color::Magenta, Color::Black, "S");

        display.drawText(100, 80, 1, Color::Green, Color::Black, "a game by the Leinis");

        display.drawText(50, 120, 2, Color::Green, Color::Black, "Enjoy the game!");
        if (!bStartButtonPressed)
        {
            display.drawText(50, 220, 2, Color::Black, Color::Yellow, "Press here to start");
        } else 
        {
            display.drawText(50, 220, 2, Color::Yellow, Color::Black, "Press here to start");
        }

        //  Draw the letter T as Tetris blocks in red
        DrawTetrisShape(display, 30, 260, Color::Red, shapeT);
        DrawTetrisShape(display, 70, 280, Color::Blue, shapeE);
        DrawTetrisShape(display, 110, 300, Color::Green, shapeT);
        DrawTetrisShape(display, 150, 320, Color::Yellow, shapeR);
        DrawTetrisShape(display, 210, 340, Color::Cyan, shapeI);
        DrawTetrisShape(display, 230, 360, Color::Magenta, shapeS);
        bMenuUpdateNeeded = false;
    }

    //  Read touchscreen input
    uint16_t x, y;
    bool touched;
    touchscreen.readValues(x, y, touched);
    if (touched)
    {
        // Check if the touch is within the "Press here to start" area
        if (x >= 50 && x <= 250 && y >= 220 && y <= 254) // Adjust the coordinates as needed
        {
            DEBUG_PRINT("Touch detected at: x=" + String(x) + ", y=" + String(y));
            if (!bStartButtonPressed)
            {
                bStartButtonPressed = true;
                bMenuUpdateNeeded = true;
            }
        } else 
        {
            DEBUG_PRINT("Touch outside start button area at: x=" + String(x) + ", y=" + String(y));
            if (bStartButtonPressed)
            {
                bStartButtonPressed = false;
                bMenuUpdateNeeded = true;
            }
        }
    } else 
    {
        if (bStartButtonPressed)
        {
            DEBUG_PRINT("Start button released, starting game.");
            bStartButtonPressed = false;
            bStartGame = true;
            return true;
        }
    }

    // Implement the logic to display the main menu here.
    // Return true if the menu shall be left, false to stay in the menu next call.
    return false;
}