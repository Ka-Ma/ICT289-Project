        /**
        @brief Draws and defines controls for Settings User Interface.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "globalState.h"
#include "dispText.h"

void displayUISettings();
void keysUISettings(unsigned char key, int x, int y);
void mouseUISettings(int button, int state, int x, int y);

#endif // UISETTINGS_H
