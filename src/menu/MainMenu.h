#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__
#include <Arduino.h>
#include "hardware/Display.h"
#include "hardware/Joystick.h"
#include "hardware/Touchscreen.h"

class MainMenu 
{
    protected:

        bool bStartGame;
        bool bMenuUpdateNeeded;

        bool bStartButtonPressed;

    public:
        MainMenu() : bStartGame(false), bMenuUpdateNeeded(true), bStartButtonPressed(false)  { }
        
        /// @brief Initializes the main menu.
        /// @details This function sets up the internal state of the main menu and prepares it for display.
        void initMenu();

        /// @brief Displays the main menu
        /// @details This function handles the rendering and interaction of the main menu.
        /// @details Changes made by the user in the menu can be queried by appropriate getter functions.
        /// @param display The display object used to render the menu.
        /// @param joystick The joystick object used to handle user input.
        /// @param touchScreen The touchscreen object used to handle touch input.
        /// @param timestamp The current timestamp used to adjust time sensitive items in the menu.
        /// @return True if the menu shall be left, false to stay in the menu next call.
        bool show(Display& display, Joystick& joystick, Touchscreen& touchScreen, uint32_t timestamp);

    
        bool getStartGame() const { return bStartGame; }
};

#endif // __MAIN_MENU_H__
