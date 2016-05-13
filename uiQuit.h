        /**
        @brief Draws and defines controls for Quit User Interface.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UIQUIT_H
#define UIQUIT_H

#include "dispText.h"
#include "globalState.h"

void displayUIQuit();
void keysUIQuit(unsigned char key, int x, int y);
void mouseUIQuit(int button, int state, int x, int y);

#endif // UIQUIT_H

