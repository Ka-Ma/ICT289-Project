        /**
        @brief Draws and defines controls for Options User Interface.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UIOPTIONS_H
#define UIOPTIONS_H

#include "dispText.h"
#include "globalState.h"
#include "uiControls.h"

//used to know if a control has been clicked
struct uiO {
    GLshort height; // height of window for mouse/window coord match up
    GLshort trackBL[2]; // toggle to track firework (automatically look at it when launched)
}uiO;

void displayUIOptions();
void keysUIOptions(unsigned char key, int x, int y);
void mouseUIOptions(int button, int state, int x, int y);

#endif // UIOPTIONS_H
