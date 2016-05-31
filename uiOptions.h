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
#include "soil.h"

//used to know if a control has been clicked
struct uiO {
    GLshort height; // height of window for mouse/window coord match up
    GLshort trackBL[2]; // check to toggle track firework (automatically look at it when launched)
    GLshort resumeBL[2]; //coords for resume and quit buttons
    GLshort resumeTR[2];
    GLshort quitBL[2];
    GLshort quitTR[2];
}uiO;

void displayUIOptions();
void keysUIOptions(unsigned char key, int x, int y);
void mouseUIOptions(int button, int state, int x, int y);

#endif // UIOPTIONS_H
